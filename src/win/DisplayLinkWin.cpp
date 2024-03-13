// DisplayLinkWin.cpp
#include "DisplayLinkWin.h"
#include <iostream>

void LogError(const char* message) { std::cerr << "DisplayLinkWin Error: " << message << std::endl; }

LRESULT CALLBACK DisplayLinkWin::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

DisplayLinkWin::DisplayLinkWin(HMONITOR hMonitor, std::function<void()> callback)
    : m_hMonitor(hMonitor)
    , m_refreshRate(60)
    , m_callback(callback)
    , m_running(false)
    , m_useD3D(false)
    , m_device(nullptr)
    , m_context(nullptr)
    , m_swapChain(nullptr)
    , m_threadHandle(nullptr)
{
    m_hInstance = GetModuleHandle(nullptr);
    m_refreshRate = queryRefreshRate();
    // TODO: Disabling D3D fo rnow.
    if (false && createHiddenWindowOnMonitor() && checkD3DAvailability() && setupD3DForVSync()) {
        m_useD3D = true;
        startD3DRenderLoop();
    } else {
        m_useD3D = false;
        startTimerLoop();
    }
}

DisplayLinkWin::~DisplayLinkWin()
{
    m_running = false;
    if (m_threadHandle) {
        WaitForSingleObject(m_threadHandle, INFINITE);
        CloseHandle(m_threadHandle);
    }
    if (m_swapChain)
        m_swapChain->Release();
    if (m_context)
        m_context->Release();
    if (m_device)
        m_device->Release();
    if (m_hwnd)
        DestroyWindow(m_hwnd);
}

bool DisplayLinkWin::createHiddenWindowOnMonitor()
{
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = L"DisplayLinkWinClass";
    if (!RegisterClass(&wc))
        return false;

    m_hwnd = CreateWindowEx(0, wc.lpszClassName, L"DisplayLinkWin Hidden Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
        nullptr, m_hInstance, nullptr);
    if (!m_hwnd)
        return false;

    MONITORINFO mi = { sizeof(mi) };
    if (GetMonitorInfo(m_hMonitor, &mi)) {
        SetWindowPos(m_hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, 0, 0, SWP_NOSIZE);
        return true;
    }
    return false;
}

bool DisplayLinkWin::checkD3DAvailability()
{
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
        D3D11_SDK_VERSION, &m_device, nullptr, &m_context);
    return SUCCEEDED(hr);
}

bool DisplayLinkWin::setupD3DForVSync()
{
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferDesc.RefreshRate.Numerator = m_refreshRate;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = m_hwnd;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;

    IDXGIDevice* dxgiDevice = nullptr;
    if (FAILED(
            m_device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice))))
        return false;

    IDXGIAdapter* dxgiAdapter = nullptr;
    dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter));

    IDXGIFactory* dxgiFactory = nullptr;
    dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory));

    HRESULT hr = dxgiFactory->CreateSwapChain(m_device, &scd, &m_swapChain);
    dxgiDevice->Release();
    dxgiAdapter->Release();
    dxgiFactory->Release();
    return SUCCEEDED(hr);
}

void DisplayLinkWin::d3dRenderLoop()
{
    m_running = true;
    while (m_running) {
        m_swapChain->Present(1, 0); // Enable VSync
        m_callback();
    }
}

void DisplayLinkWin::timerLoop()
{
    HANDLE timer = CreateWaitableTimer(nullptr, FALSE, nullptr);
    if (!timer) {
        LogError("Failed to create waitable timer.");
        return;
    }

    // Set the timer to fire at a rate slightly higher than the refresh rate
    int targetFPS = static_cast<int>(m_refreshRate * 1.2 + 10);

    LARGE_INTEGER dueTime;
    dueTime.QuadPart = -static_cast<LONGLONG>(10000.0 * 1000 / targetFPS);
    if (!SetWaitableTimer(timer, &dueTime, 1000 / m_refreshRate, nullptr, nullptr, FALSE)) {
        LogError("Failed to set waitable timer.");
        CloseHandle(timer);
        return;
    }

    m_running = true;
    while (m_running) {
        if (WaitForSingleObject(timer, INFINITE) == WAIT_OBJECT_0) {
            m_callback();
        }
    }

    CancelWaitableTimer(timer);
    CloseHandle(timer);
}

int DisplayLinkWin::queryRefreshRate()
{
    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);

    // Attempt to get monitor information
    if (GetMonitorInfo(m_hMonitor, &mi)) {
        DEVMODE dm;
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        // Enumerate display settings for the specific monitor
        if (EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dm)) {
            return dm.dmDisplayFrequency; // Return the actual refresh rate
        }
    }
    return 60; // Fallback to 60 Hz if unable to get the information
}

void DisplayLinkWin::startD3DRenderLoop()
{
    m_running = true;
    DWORD threadId;
    m_threadHandle = CreateThread(
        nullptr, 0,
        [](LPVOID param) -> DWORD {
            auto* instance = reinterpret_cast<DisplayLinkWin*>(param);
            instance->d3dRenderLoop();
            return 0;
        },
        this, 0, &threadId);
}

void DisplayLinkWin::startTimerLoop()
{
    m_running = true;
    DWORD threadId;
    m_threadHandle = CreateThread(
        nullptr, 0,
        [](LPVOID param) -> DWORD {
            auto* instance = reinterpret_cast<DisplayLinkWin*>(param);
            instance->timerLoop();
            return 0;
        },
        this, 0, &threadId);
}
