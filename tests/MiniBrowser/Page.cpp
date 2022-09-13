#include "Page.h"
#include "UI.h"
#include <iostream>
#include <string>

#define INSPECTOR_DRAG_HANDLE_HEIGHT 10

Page::Page(UI* ui, uint32_t width, uint32_t height, int x, int y) 
  : ui_(ui), container_width_(width), container_height_(height) {
  overlay_ = Overlay::Create(ui->window_, width, height, x, y);
  view()->set_view_listener(this);
  view()->set_load_listener(this);
  view()->set_download_listener(this);
}

Page::~Page() {
  view()->set_view_listener(nullptr);
  view()->set_load_listener(nullptr);
  view()->set_download_listener(nullptr);
}

void Page::Show() {
  overlay_->Show();
  overlay_->Focus();

  if (inspector_overlay_)
    inspector_overlay_->Show();
}

void Page::Hide() {
  overlay_->Hide();
  overlay_->Unfocus();

  if (inspector_overlay_)
    inspector_overlay_->Hide();
}

void Page::ToggleInspector() {
  if (!inspector_overlay_) {
    view()->CreateLocalInspectorView();
  } else {
    if (inspector_overlay_->is_hidden()) {
      inspector_overlay_->Show();
    } else {
      inspector_overlay_->Hide();
    }
  }

  // Force resize to update layout
  Resize(container_width_, container_height_);
}

bool Page::IsInspectorShowing() const {
  if (!inspector_overlay_)
    return false;

  return !inspector_overlay_->is_hidden();
}

IntRect Page::GetInspectorResizeDragHandle() const {
  if (!IsInspectorShowing())
    return IntRect::MakeEmpty();

  int drag_handle_height_px = (uint32_t)std::round(INSPECTOR_DRAG_HANDLE_HEIGHT * ui_->window()->scale());

  // This drag handle should span the width of the UI and be centered vertically at the boundary between
  // the page overlay and inspector overlay.

  int drag_handle_x = (int)inspector_overlay_->x();
  int drag_handle_y = (int)inspector_overlay_->y() - drag_handle_height_px / 2;

  return { drag_handle_x, drag_handle_y, drag_handle_x + (int)inspector_overlay_->width(),
           drag_handle_y + drag_handle_height_px };
}

int Page::GetInspectorHeight() const {
  if (inspector_overlay_)
    return inspector_overlay_->height();

  return 0;
}

void Page::SetInspectorHeight(int height) {
  if (height > 2) {
    inspector_overlay_->Resize(inspector_overlay_->width(), height);

    // Trigger a resize to perform re-layout / re-size of content overlay
    Resize(container_width_, container_height_);
  }
}

void Page::Resize(uint32_t width, uint32_t height) {
  container_width_ = width;
  container_height_ = height;

  uint32_t content_height = container_height_;
  if (inspector_overlay_ && !inspector_overlay_->is_hidden()) {
    uint32_t inspector_height_px = inspector_overlay_->height();
    content_height -= inspector_height_px;
  }
  
  if (content_height < 1)
    content_height = 1;

  overlay_->Resize(container_width_, content_height);

  if (inspector_overlay_ && !inspector_overlay_->is_hidden())
    inspector_overlay_->MoveTo(0, overlay_->y() + overlay_->height());
}

void Page::OnChangeTitle(View* caller, const String& title) {
  ui_->SetTitle(title);
}

void Page::OnChangeURL(View* caller, const String& url) {
  ui_->SetURL( url);
}

void Page::OnChangeTooltip(View* caller, const String& tooltip) {}

void Page::OnChangeCursor(View* caller, Cursor cursor) {
  ui_->SetCursor(cursor);
}

void Page::OnAddConsoleMessage(View* caller,
  MessageSource source,
  MessageLevel level,
  const String& message,
  uint32_t line_number,
  uint32_t column_number,
  const String& source_id) {
}

RefPtr<View> Page::OnCreateChildView(ultralight::View* caller,
  const String& opener_url, const String& target_url,
  bool is_popup, const IntRect& popup_rect) {
  
  // TODO: handle child view in new window
  return nullptr;
}

RefPtr<View> Page::OnCreateInspectorView(ultralight::View* caller, bool is_local,
                                         const String& inspected_url) {
  if (inspector_overlay_)
    return nullptr;

  inspector_overlay_ = Overlay::Create(ui_->window_, container_width_, container_height_ / 2, 0, 0);

  // Force resize to update layout
  Resize(container_width_, container_height_);
  inspector_overlay_->Show();

  return inspector_overlay_->view();
}

void Page::OnBeginLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
  ui_->UpdatePageNavigation(caller->is_loading(), caller->CanGoBack(), caller->CanGoForward());

  std::cout << "[OnBeginLoading]\n\t"
            << "\n\tframe_id:\t" << frame_id
            << "\n\tis_main_frame:\t" << is_main_frame
            << "\n\turl:\t" << url.utf8().data()
            << std::endl;
}

void Page::OnFinishLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url) {
  ui_->UpdatePageNavigation(caller->is_loading(), caller->CanGoBack(), caller->CanGoForward());

  std::cout << "[OnFinishLoading]\n\t"
        << "\n\tframe_id:\t" << frame_id << "\n\tis_main_frame:\t" << is_main_frame
        << "\n\turl:\t" << url.utf8().data() << std::endl;
}

void Page::OnFailLoading(View* caller, uint64_t frame_id, bool is_main_frame, const String& url,
  const String& description, const String& error_domain, int error_code) {
  if (is_main_frame && error_code != 102) {
    char error_code_str[16]; 
    sprintf(error_code_str,"%d", error_code);

    String html_string = "<html><head><style>";
    html_string += "* { font-family: sans-serif; }";
    html_string += "body { background-color: #CCC; color: #555; padding: 4em; }";
    html_string += "dt { font-weight: bold; padding: 1em; }";
    html_string += "</style></head><body>";
    html_string += "<h2>A Network Error was Encountered</h2>";
    html_string += "<dl>";
    html_string += "<dt>URL</dt><dd>" + url + "</dd>";
    html_string += "<dt>Description</dt><dd>" + description + "</dd>";
    html_string += "<dt>Error Domain</dt><dd>" + error_domain + "</dd>";
    html_string += "<dt>Error Code</dt><dd>" + String(error_code_str) + "</dd>";
    html_string += "</dl></body></html>";

    view()->LoadHTML(html_string);
  }

  std::cout << "[OnFailLoading]\n\t"
            << "\n\tframe_id:\t" << frame_id << "\n\tis_main_frame:\t" << is_main_frame
            << "\n\turl:\t" << url.utf8().data() << "\n\tdescription:\t"
            << description.utf8().data() << "\n\terror_domain:\t" << error_domain.utf8().data()
            << "\n\terror_code:\t" << error_code << std::endl;
}

void Page::OnWindowObjectReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                                 const String& url) {
  std::cout << "[OnWindowObjectReady]\n\t"
            << "\n\tframe_id:\t" << frame_id << "\n\tis_main_frame:\t" << is_main_frame
            << "\n\turl:\t" << url.utf8().data() << std::endl;
}

void Page::OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
                        const String& url) {
  std::cout << "[OnDOMReady]\n\t"
            << "\n\tframe_id:\t" << frame_id << "\n\tis_main_frame:\t" << is_main_frame
            << "\n\turl:\t" << url.utf8().data() << std::endl;
}

void Page::OnUpdateHistory(View* caller) {
  ui_->UpdatePageNavigation(caller->is_loading(), caller->CanGoBack(), caller->CanGoForward());
}

bool Page::OnRequestDownload(ultralight::View* caller, DownloadId id, const String& url) {
  std::cout << "[OnRequestDownload]" << std::endl;
  return true;
}

void Page::OnBeginDownload(ultralight::View* caller, DownloadId id, const String& url,
                           const String& filename, int64_t expected_content_length) {
  std::cout << "[OnBeginDownload] id: " << id << ", url: " << url.utf8().data()
            << ", filename: " << filename.utf8().data() << ", length: " << expected_content_length
            << std::endl;

  std::ofstream outFile;
  outFile.open(filename.utf8().data(), std::ios::out | std::ios::app | std::ios::binary); 

  // TODO: Write downloads to actual download folder instead of current working directory
  if (outFile.is_open()) {
    active_downloads_.insert({ id, std::move(outFile) });
  } else {
    std::cout << "Error: Unable to open file for writing: " << filename.utf8().data() << std::endl;
  }
}

void Page::OnReceiveDataForDownload(ultralight::View* caller, DownloadId id,
                                    RefPtr<Buffer> data) { 
  //std::cout << "[OnReceiveDataForDownload] id: " << id << std::endl;
  auto download = active_downloads_.find(id);
  if (download != active_downloads_.end()) {
    download->second.write((const char*)data->data(), data->size());
  }
}

void Page::OnFinishDownload(ultralight::View* caller, DownloadId id) {
  std::cout << "[OnFinishDownload] id: " << id << std::endl;
  auto download = active_downloads_.find(id);
  if (download != active_downloads_.end()) {
    download->second.close();
  }
}

void Page::OnFailDownload(ultralight::View* caller, DownloadId id) {
  std::cout << "[OnFailDownload] id: " << id << std::endl;
  auto download = active_downloads_.find(id);
  if (download != active_downloads_.end()) {
    download->second.close();
  }
}