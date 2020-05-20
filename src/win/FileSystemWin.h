#pragma once
#include <Ultralight/platform/FileSystem.h>
#include <Windows.h>
#include <memory>

namespace ultralight {

/**
 * FileSystem implementation for Windows.
 */
class FileSystemWin : public FileSystem {
public:
  // Construct FileSystemWin instance.
  // 
  // @note You can pass a valid baseDir here which will be prepended to
  //       all file paths. This is useful for making all File URLs relative
  //       to your HTML asset directory.
  FileSystemWin(LPCWSTR baseDir);

  virtual ~FileSystemWin();

  virtual bool FileExists(const String16& path) override;

  virtual bool GetFileSize(FileHandle handle, int64_t& result) override;

  virtual bool GetFileMimeType(const String16& path, String16& result) override;

  virtual FileHandle OpenFile(const String16& path, bool open_for_writing) override;

  virtual void CloseFile(FileHandle& handle) override;

  virtual int64_t ReadFromFile(FileHandle handle, char* data, int64_t length) override;

protected:
  std::unique_ptr<WCHAR[]> GetRelative(const String16& path);

  std::unique_ptr<WCHAR[]> baseDir_;
};

}  // namespace ultralight
