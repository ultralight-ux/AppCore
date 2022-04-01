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

  virtual bool FileExists(const String& file_path) override;

  virtual String GetFileMimeType(const String& file_path) override;

  virtual String GetFileCharset(const String& file_path) override;

  virtual RefPtr<Buffer> OpenFile(const String& file_path) override;

protected:
  std::unique_ptr<WCHAR[]> GetRelative(const String& path);

  std::unique_ptr<WCHAR[]> baseDir_;
};

}  // namespace ultralight
