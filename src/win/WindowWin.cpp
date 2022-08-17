#include "WindowWin.h"
#include "AppWin.h"
#include "DIBSurface.h"
#include "AppCore/Overlay.h"
#include "Windowsx.h"
#include <AppCore/Monitor.h>
#include <Ultralight/private/tracy/Tracy.hpp>
#include <ShellScalingAPI.h>
#include <Windows.h>
#include <tchar.h>
#if defined(DRIVER_D3D11)
#include "d3d11/GPUContextD3D11.h"
#include "d3d11/GPUDriverD3D11.h"
#elif defined(DRIVER_D3D12)
#include "d3d12/GPUContextD3D12.h"
#include "d3d12/GPUDriverD3D12.h"
#endif

namespace ultralight {

#define WINDOWDATA() ((WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA))
#define WINDOW() (WINDOWDATA()->window)
#define WM_DPICHANGED_ 0x02E0

static HDC g_dc = 0;

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;
  switch (message) {
  case WM_PAINT:
    g_dc = hdc = BeginPaint(hWnd, &ps);
    WINDOW()->InvalidateWindow();
    WINDOW()->Paint();
    EndPaint(hWnd, &ps);
    g_dc = 0;
    break;
  case WM_DESTROY:
    WINDOW()->OnClose();
    break;
  case WM_ENTERSIZEMOVE:
    WINDOWDATA()->is_resizing_modal = true;
    break;
  case WM_SIZE: {
    if (WINDOWDATA()) {
      WINDOW()->OnResize(WINDOW()->width(), WINDOW()->height());
      // This would normally be called when the message loop is idle
      // but during resize the window consumes all messages so we need
      // to force paints during the operation.
      // static_cast<AppWin*>(App::instance())->OnPaint();
      InvalidateRect(hWnd, nullptr, false);
    }
    break;
  }
  case WM_DPICHANGED_: {
    if (WINDOWDATA()) {
      double fscale = (double)HIWORD(wParam) / USER_DEFAULT_SCREEN_DPI;
      WINDOW()->OnChangeDPI(fscale, (RECT*)lParam);
      InvalidateRect(hWnd, nullptr, false);
    }
  }
  case WM_EXITSIZEMOVE:
    WINDOWDATA()->is_resizing_modal = false;
    WINDOW()->OnResize(WINDOW()->width(), WINDOW()->height());
    InvalidateRect(hWnd, nullptr, false);
    break;
  case WM_KEYDOWN:
    WINDOW()->FireKeyEvent(
        KeyEvent(KeyEvent::kType_RawKeyDown, (uintptr_t)wParam, (intptr_t)lParam, false));
    break;
  case WM_KEYUP:
    WINDOW()->FireKeyEvent(
        KeyEvent(KeyEvent::kType_KeyUp, (uintptr_t)wParam, (intptr_t)lParam, false));
    break;
  case WM_CHAR:
    WINDOW()->FireKeyEvent(
        KeyEvent(KeyEvent::kType_Char, (uintptr_t)wParam, (intptr_t)lParam, false));
    break;
  case WM_MOUSELEAVE:
    WINDOWDATA()->is_mouse_in_client = false;
    WINDOWDATA()->is_tracking_mouse = false;
    break;
  case WM_MOUSEMOVE: {
    if (!WINDOWDATA()->is_tracking_mouse) {
      // Need to install tracker to get WM_MOUSELEAVE events.
      WINDOWDATA()->track_mouse_event_data = { sizeof(WINDOWDATA()->track_mouse_event_data) };
      WINDOWDATA()->track_mouse_event_data.dwFlags = TME_LEAVE;
      WINDOWDATA()->track_mouse_event_data.hwndTrack = hWnd;
      TrackMouseEvent(&(WINDOWDATA()->track_mouse_event_data));
      WINDOWDATA()->is_tracking_mouse = true;
    }
    if (!WINDOWDATA()->is_mouse_in_client) {
      // We need to manually set the cursor when mouse enters client area
      WINDOWDATA()->is_mouse_in_client = true;
      WINDOW()->SetCursor(ultralight::kCursor_Pointer);
    }
    WINDOW()->FireMouseEvent(
        { MouseEvent::kType_MouseMoved, WINDOW()->PixelsToScreen(GET_X_LPARAM(lParam)),
          WINDOW()->PixelsToScreen(GET_Y_LPARAM(lParam)), WINDOWDATA()->cur_btn });
    break;
  }
  case WM_LBUTTONDOWN:
  case WM_LBUTTONDBLCLK:
    SetCapture(WINDOW()->hwnd());
    WINDOWDATA()->cur_btn = MouseEvent::kButton_Left;
    WINDOW()->FireMouseEvent(
        { MouseEvent::kType_MouseDown, WINDOW()->PixelsToScreen(GET_X_LPARAM(lParam)),
          WINDOW()->PixelsToScreen(GET_Y_LPARAM(lParam)), WINDOWDATA()->cur_btn });
    break;
  case WM_MBUTTONDOWN:
  case WM_MBUTTONDBLCLK:
    SetCapture(WINDOW()->hwnd());
    WINDOWDATA()->cur_btn = MouseEvent::kButton_Middle;
    WINDOW()->FireMouseEvent(
        { MouseEvent::kType_MouseDown, WINDOW()->PixelsToScreen(GET_X_LPARAM(lParam)),
          WINDOW()->PixelsToScreen(GET_Y_LPARAM(lParam)), WINDOWDATA()->cur_btn });
    break;
  case WM_RBUTTONDOWN:
  case WM_RBUTTONDBLCLK:
    SetCapture(WINDOW()->hwnd());
    WINDOWDATA()->cur_btn = MouseEvent::kButton_Right;
    WINDOW()->FireMouseEvent(
        { MouseEvent::kType_MouseDown, WINDOW()->PixelsToScreen(GET_X_LPARAM(lParam)),
          WINDOW()->PixelsToScreen(GET_Y_LPARAM(lParam)), WINDOWDATA()->cur_btn });
    break;
  case WM_LBUTTONUP:
  case WM_MBUTTONUP:
  case WM_RBUTTONUP:
    ReleaseCapture();
    WINDOW()->FireMouseEvent(
        { MouseEvent::kType_MouseUp, WINDOW()->PixelsToScreen(GET_X_LPARAM(lParam)),
          WINDOW()->PixelsToScreen(GET_Y_LPARAM(lParam)), WINDOWDATA()->cur_btn });
    WINDOWDATA()->cur_btn = MouseEvent::kButton_None;
    break;
  case WM_MOUSEWHEEL:
    WINDOW()->FireScrollEvent(
        { ScrollEvent::kType_ScrollByPixel, 0,
          static_cast<int>(WINDOW()->PixelsToScreen(GET_WHEEL_DELTA_WPARAM(wParam)) * 0.8) });
    break;
  case WM_SETFOCUS:
    WINDOW()->SetWindowFocused(true);
    break;
  case WM_KILLFOCUS:
    WINDOW()->SetWindowFocused(false);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

void CenterHwndOnMainMonitor(HWND hwnd) {
  RECT rect;
  GetWindowRect(hwnd, &rect);
  LPRECT prc = &rect;

  // Get main monitor
  HMONITOR hMonitor = MonitorFromPoint({ 1, 1 }, MONITOR_DEFAULTTONEAREST);

  MONITORINFO mi;
  RECT rc;
  int w = prc->right - prc->left;
  int h = prc->bottom - prc->top;

  mi.cbSize = sizeof(mi);
  GetMonitorInfo(hMonitor, &mi);

  rc = mi.rcMonitor;

  prc->left = rc.left + (rc.right - rc.left - w) / 2;
  prc->top = rc.top + (rc.bottom - rc.top - h) / 2;
  prc->right = prc->left + w;
  prc->bottom = prc->top + h;

  SetWindowPos(hwnd, NULL, rect.left, rect.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

static bool g_window_class_initialized = false;

WindowWin::WindowWin(Monitor* monitor, uint32_t width, uint32_t height, bool fullscreen,
                     unsigned int window_flags)
    : monitor_(monitor), is_fullscreen_(fullscreen) {

  HINSTANCE hInstance = GetModuleHandle(NULL);
  TCHAR* class_name = _T("UltralightWindow");

  if (!g_window_class_initialized) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_APPLICATION);
    wcex.hCursor = NULL;
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = class_name;
    wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_APPLICATION);
    if (!RegisterClassEx(&wcex)) {
      MessageBoxW(NULL, (LPCWSTR)L"RegisterClassEx failed", (LPCWSTR)L"Notification", MB_OK);
      exit(-1);
    }
    g_window_class_initialized = true;
  }

  style_ = WS_SYSMENU;
  if (window_flags & kWindowFlags_Borderless)
    style_ |= WS_POPUP;
  else
    style_ |= WS_BORDER;
  if (window_flags & kWindowFlags_Titled)
    style_ |= WS_CAPTION;
  if (window_flags & kWindowFlags_Resizable)
    style_ |= WS_SIZEBOX;
  if (window_flags & kWindowFlags_Maximizable)
    style_ |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

  scale_ = monitor_->scale();

  // Create window
  RECT rc = { 0, 0, (LONG)ScreenToPixels(width), (LONG)ScreenToPixels(height) };
  AdjustWindowRect(&rc, style_, FALSE);
  hwnd_ = ::CreateWindowEx(
      NULL, class_name, _T(""), fullscreen ? (WS_EX_TOPMOST | WS_POPUP) : style_,
      fullscreen ? 0 : CW_USEDEFAULT, fullscreen ? 0 : CW_USEDEFAULT,
      fullscreen ? ScreenToPixels(width) : (rc.right - rc.left),
      fullscreen ? ScreenToPixels(height) : (rc.bottom - rc.top), NULL, NULL, hInstance, NULL);

  if (!hwnd_) {
    MessageBoxW(NULL, (LPCWSTR)L"CreateWindowEx failed", (LPCWSTR)L"Notification", MB_OK);
    exit(-1);
  }

  window_data_.window = this;
  window_data_.cur_btn = ultralight::MouseEvent::kButton_None;
  window_data_.is_resizing_modal = false;
  window_data_.is_mouse_in_client = false;
  window_data_.is_tracking_mouse = false;

  SetWindowLongPtr(hwnd_, GWLP_USERDATA, (LONG_PTR)&window_data_);

  // CenterHwndOnMainMonitor(hwnd_);

  if (!(window_flags & kWindowFlags_Hidden))
    ShowWindow(hwnd_, SW_SHOW);

  // Set the thread affinity mask for better clock
  ::SetThreadAffinityMask(::GetCurrentThread(), 1);

  cursor_hand_ = ::LoadCursor(NULL, IDC_HAND);
  cursor_arrow_ = ::LoadCursor(NULL, IDC_ARROW);
  cursor_ibeam_ = ::LoadCursor(NULL, IDC_IBEAM);
  cursor_size_all_ = ::LoadCursor(NULL, IDC_SIZEALL);
  cursor_size_north_east_ = ::LoadCursor(NULL, IDC_SIZENESW);
  cursor_size_north_south_ = ::LoadCursor(NULL, IDC_SIZENS);
  cursor_size_north_west_ = ::LoadCursor(NULL, IDC_SIZENWSE);
  cursor_size_west_east_ = ::LoadCursor(NULL, IDC_SIZEWE);
  cur_cursor_ = ultralight::kCursor_Pointer;

  SetWindowScale(scale());

  is_accelerated_ = false;

  assert(App::instance());

#if defined(DRIVER_D3D11)
  auto gpu_context = static_cast<AppWin*>(App::instance())->gpu_context();
  auto gpu_driver = static_cast<AppWin*>(App::instance())->gpu_driver();
  if (gpu_context && gpu_driver) {
    swap_chain_.reset(new SwapChainD3D11(gpu_context, gpu_driver, hwnd_, this->width(),
                                         this->height(), scale(), is_fullscreen(), true, true, 1));
    if (swap_chain_->swap_chain()) {
      is_accelerated_ = true;
      gpu_context->AddSwapChain(swap_chain_.get());
    } else {
      swap_chain_.reset();
    }
  }
#endif

  static_cast<AppWin*>(App::instance())->AddWindow(this);
}

WindowWin::~WindowWin() {
  DestroyCursor(cursor_hand_);
  DestroyCursor(cursor_arrow_);
  DestroyCursor(cursor_ibeam_);
  DestroyCursor(cursor_size_all_);
  DestroyCursor(cursor_size_north_east_);
  DestroyCursor(cursor_size_north_south_);
  DestroyCursor(cursor_size_north_west_);
  DestroyCursor(cursor_size_west_east_);

  if (App::instance()) {
    static_cast<AppWin*>(App::instance())->RemoveWindow(this);

#if defined(DRIVER_D3D11)
    auto gpu_context = static_cast<AppWin*>(App::instance())->gpu_context();
    if (is_accelerated_ && gpu_context && swap_chain_) {
      gpu_context->RemoveSwapChain(swap_chain_.get());
    }
#endif
  }
}

uint32_t WindowWin::screen_width() const { return PixelsToScreen(width()); }

uint32_t WindowWin::width() const {
  RECT rc;
  ::GetClientRect(hwnd_, &rc);
  return rc.right - rc.left;
}

uint32_t WindowWin::screen_height() const { return PixelsToScreen(height()); }

uint32_t WindowWin::height() const {
  RECT rc;
  ::GetClientRect(hwnd_, &rc);
  return rc.bottom - rc.top;
}

double WindowWin::scale() const { return scale_; }

void WindowWin::MoveTo(int x, int y) {
  x = ScreenToPixels(x);
  y = ScreenToPixels(y);
  RECT rect = { x, y, x, y };
  AdjustWindowRect(&rect, style_, FALSE);
  SetWindowPos(hwnd_, NULL, rect.left, rect.top, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
}

void WindowWin::MoveToCenter() { CenterHwndOnMainMonitor(hwnd_); }

int WindowWin::x() const {
  POINT pos = { 0, 0 };
  ClientToScreen(hwnd_, &pos);

  return PixelsToScreen(pos.x);
}

int WindowWin::y() const {
  POINT pos = { 0, 0 };
  ClientToScreen(hwnd_, &pos);

  return PixelsToScreen(pos.y);
}

void WindowWin::SetTitle(const char* title) { SetWindowTextA(hwnd_, title); }

void WindowWin::SetCursor(ultralight::Cursor cursor) {
  switch (cursor) {
  case ultralight::kCursor_Hand: {
    ::SetCursor(cursor_hand_);
    break;
  }
  case ultralight::kCursor_Pointer: {
    ::SetCursor(cursor_arrow_);
    break;
  }
  case ultralight::kCursor_IBeam: {
    ::SetCursor(cursor_ibeam_);
    break;
  }
  case ultralight::kCursor_Move: {
    ::SetCursor(cursor_size_all_);
    break;
  }
  case ultralight::kCursor_NorthEastResize:
  case ultralight::kCursor_SouthWestResize:
  case ultralight::kCursor_NorthEastSouthWestResize: {
    ::SetCursor(cursor_size_north_east_);
    break;
  }
  case ultralight::kCursor_NorthResize:
  case ultralight::kCursor_SouthResize:
  case ultralight::kCursor_NorthSouthResize: {
    ::SetCursor(cursor_size_north_south_);
    break;
  }
  case ultralight::kCursor_NorthWestResize:
  case ultralight::kCursor_SouthEastResize:
  case ultralight::kCursor_NorthWestSouthEastResize: {
    ::SetCursor(cursor_size_north_east_);
    break;
  }
  case ultralight::kCursor_WestResize:
  case ultralight::kCursor_EastResize:
  case ultralight::kCursor_EastWestResize: {
    ::SetCursor(cursor_size_west_east_);
    break;
  }
  };

  cur_cursor_ = cursor;
}

void WindowWin::Show() { ShowWindow(hwnd_, SW_SHOW); }

void WindowWin::Hide() { ShowWindow(hwnd_, SW_HIDE); }

bool WindowWin::is_visible() const { return IsWindowVisible(hwnd_); }

void WindowWin::Close() { DestroyWindow(hwnd_); }

/*
void WindowWin::DrawBitmap(int x, int y, RefPtr<Bitmap> bitmap, IntRect rect) {
  if (bitmap->IsEmpty())
    return;

  //HDC hdc = GetDC(hwnd_);
  if (!g_dc) return;
  HDC hdc = g_dc;
  void* bits = bitmap->LockPixels();
  BITMAPINFO bmi = {};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
  bmi.bmiHeader.biWidth = static_cast<LONG>(bitmap->width());
  bmi.bmiHeader.biHeight = -static_cast<LONG>(bitmap->height());
  bmi.bmiHeader.biBitCount = bitmap->bpp() * 8;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biSizeImage = static_cast<DWORD>(bitmap->size());

  // TODO: Handle double buffering properly. For now ignore dirty rect bounds
  //       and force repaint of entire bitmap.
  rect = { 0, 0, (int)bitmap->width(), (int)bitmap->height() };

  LONG adjustedSourceTop = bitmap->height() - rect.bottom;
  StretchDIBits(hdc, x + rect.left, y + rect.top, rect.width(), rect.height(),
    rect.left, adjustedSourceTop, rect.width(), rect.height(), bits, &bmi, DIB_RGB_COLORS, SRCCOPY);

  bitmap->UnlockPixels();
}
*/

void WindowWin::DrawSurface(int x, int y, Surface* surface) {
  DIBSurface* dibSurface = static_cast<DIBSurface*>(surface);

  if (!g_dc)
    return;
  HDC hdc = g_dc;
  BitBlt(hdc, x, y, (int)surface->width(), (int)surface->height(), dibSurface->dc(), 0, 0, SRCCOPY);
}

void* WindowWin::native_handle() const { return hwnd_; }

void WindowWin::Paint() {
  const char* frame_mark_paint = "Paint";

  if (!is_accelerated()) {
    FrameMarkStart(frame_mark_paint);
    OverlayManager::Render();
    OverlayManager::Paint();
    FrameMarkEnd(frame_mark_paint);
    return;
  }

  FrameMarkStart(frame_mark_paint);

  auto gpu_context = static_cast<AppWin*>(App::instance())->gpu_context();
  auto gpu_driver = static_cast<AppWin*>(App::instance())->gpu_driver();

  gpu_driver->BeginSynchronize();
  OverlayManager::Render();
  gpu_driver->EndSynchronize();

  if (gpu_driver->HasCommandsPending() || OverlayManager::NeedsRepaint()
      || (window_needs_repaint_ && !is_first_paint_)) {
    gpu_context->BeginDrawing();
    gpu_driver->DrawCommandList();
    OverlayManager::Paint();
    swap_chain_->PresentFrame();
    gpu_context->EndDrawing();
    if (is_first_paint_)
      is_first_paint_ = false;
  }

  window_needs_repaint_ = false;

  FrameMarkEnd(frame_mark_paint);
}

void WindowWin::FireKeyEvent(const ultralight::KeyEvent& evt) {
  if (listener())
    if (!listener()->OnKeyEvent(evt))
      return;

  OverlayManager::FireKeyEvent(evt);
}

void WindowWin::FireMouseEvent(const ultralight::MouseEvent& evt) {
  if (listener())
    if (!listener()->OnMouseEvent(evt))
      return;

  OverlayManager::FireMouseEvent(evt);
}


void WindowWin::FireScrollEvent(const ultralight::ScrollEvent& evt) {
  if (listener())
    if (!listener()->OnScrollEvent(evt))
      return;

  OverlayManager::FireScrollEvent(evt);
}


void WindowWin::OnClose() {
  // Keep window alive in case user-callbacks release our reference.
  RefPtr<WindowWin> retain(this);

  if (listener_)
    listener_->OnClose(this);
  if (app_listener_)
    app_listener_->OnClose(this);
}

void WindowWin::OnResize(uint32_t width, uint32_t height) {
  // Keep window alive in case user-callbacks release our reference.
  RefPtr<WindowWin> retain(this);

  if (swap_chain_)
    swap_chain_->Resize(width, height);

  if (listener_)
    listener_->OnResize(this, width, height);

  if (app_listener_)
    app_listener_->OnResize(this, width, height);
}

void WindowWin::OnChangeDPI(double scale, const RECT* suggested_rect) {
  scale_ = scale;

  SetWindowScale(scale_);

  if (is_accelerated()) {
    swap_chain_->set_scale(scale_);
  }

  for (auto overlay : overlays_) {
    overlay->view()->set_device_scale(scale_);
  }

  RECT* const prcNewWindow = (RECT*)suggested_rect;
  SetWindowPos(hwnd_, NULL, prcNewWindow->left, prcNewWindow->top,
               prcNewWindow->right - prcNewWindow->left, prcNewWindow->bottom - prcNewWindow->top,
               SWP_NOZORDER | SWP_NOACTIVATE);
}

void WindowWin::CleanupWindowClass() {
  HINSTANCE hInstance = GetModuleHandle(NULL);
  TCHAR* class_name = _T("UltralightWindow");

  if (g_window_class_initialized) {
    UnregisterClass(class_name, hInstance);
    g_window_class_initialized = false;
  }
}

RefPtr<Window> Window::Create(Monitor* monitor, uint32_t width, uint32_t height, bool fullscreen,
                              unsigned int window_flags) {
  return AdoptRef(*new WindowWin(monitor, width, height, fullscreen, window_flags));
}

Window::~Window() { }



} // namespace ultralight
