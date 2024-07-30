#include "FileSystemBasic.h"
#include "FileUtils.h"
#include <Ultralight/String.h>
#include <fstream>
#include <memory>
#include <vector>
#include <cstring>

namespace ultralight {

// Replace all back-slashes with forward-slashes
static inline void NormalizePath(std::string& str) {
  if (!str.empty()) {
    for (size_t i = 0; i < str.length(); ++i) {
      if (str[i] == '\\')
        str[i] = '/';
    }
  }
}

FileSystemBasic::FileSystemBasic(const char* baseDir) : baseDir_(baseDir) {
  NormalizePath(baseDir_);
  if (!baseDir_.empty()) {
    // Make sure directory path ends with a slash
    if (baseDir_[baseDir_.length() - 1] != '/')
      baseDir_ += std::string("/");
  }
}

std::string FileSystemBasic::getRelative(const String& path) {
  if (path.empty())
    return baseDir_;

  String8 utf8 = String(path).utf8();
  std::string relPath(utf8.data(), utf8.length());

  NormalizePath(relPath);
  // Strip any slash from beginning of path:
  if (relPath[0] == '/')
    relPath = relPath.substr(1);

  return baseDir_ + relPath;
}

bool FileSystemBasic::FileExists(const String& path) {
  std::ifstream filestream(getRelative(path));
  return filestream.good();
}

String FileSystemBasic::GetFileMimeType(const String& file_path) {
  String8 utf8 = file_path.utf8();
  std::string filepath(utf8.data(), utf8.length()); 
  std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
  return String(FileUtils::FileExtensionToMimeType(ext.c_str()));
}

String FileSystemBasic::GetFileCharset(const String& file_path) {
  return "utf-8";
}

struct FileSystemBasic_BufferData {
  std::unique_ptr<std::ifstream> file_handle;
  int64_t file_size;
  std::unique_ptr<std::vector<char>> file_contents;
};

void FileSystemBasic_DestroyBufferCallback(void* user_data, void* data) {
  FileSystemBasic_BufferData* buffer_data = reinterpret_cast<FileSystemBasic_BufferData*>(user_data);
  buffer_data->file_contents.reset();
  buffer_data->file_handle.reset();
  delete buffer_data;
}

RefPtr<Buffer> FileSystemBasic::OpenFile(const String& file_path) {
  std::unique_ptr<std::ifstream> file(new std::ifstream(getRelative(file_path), std::ifstream::ate | std::ifstream::binary));
  if (!file->good())
    return nullptr;

  file->seekg(0, file->end);
  int64_t file_size = (int64_t)file->tellg();

  if (file_size <= 0)
    return nullptr;

  std::unique_ptr<std::vector<char>> buffer(new std::vector<char>(file_size));

  file->seekg(0, file->beg);
  file->read(buffer->data(), (std::streamsize)file_size);

  FileSystemBasic_BufferData* buffer_data = new FileSystemBasic_BufferData();
  buffer_data->file_handle = std::move(file);
  buffer_data->file_size = file_size;
  buffer_data->file_contents = std::move(buffer);
  
  return Buffer::Create(buffer_data->file_contents->data(), buffer_data->file_size,
    buffer_data, FileSystemBasic_DestroyBufferCallback);
}

FileSystem* CreatePlatformFileSystem(const String& baseDir) {
  return new ultralight::FileSystemBasic(baseDir.utf8().data());
}

}  // namespace ultralight
