#pragma once
#include <Ultralight/platform/FileSystem.h>

namespace ultralight {

class FileSystemMac : public FileSystem {
 public:
    FileSystemMac(const String& baseDir);
    virtual ~FileSystemMac();
    
    virtual bool FileExists(const String& path) override;

    virtual String GetFileMimeType(const String& file_path) override;

    virtual String GetFileCharset(const String& file_path) override;

    virtual RefPtr<Buffer> OpenFile(const String& file_path) override;
  
protected:
    String16 base_dir_;
    
    String16 getRelative(const String16& path);
};
    
}  // namespace ultralight

    
