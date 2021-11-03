#pragma once
#include <Ultralight/platform/FileSystem.h>

namespace ultralight {

class FileSystemMac : public FileSystem {
 public:
    FileSystemMac(const String& baseDir);
    virtual ~FileSystemMac();
    
    virtual bool FileExists(const String& path);

    virtual bool GetFileSize(FileHandle handle, int64_t& result);

    virtual bool GetFileMimeType(const String& path, String& result);

    virtual FileHandle OpenFile(const String& path, bool open_for_writing);

    virtual void CloseFile(FileHandle& handle);

    virtual int64_t ReadFromFile(FileHandle handle, char* data, int64_t length);
  
protected:
    String16 base_dir_;
    
    String16 getRelative(const String& path);
};
    
}  // namespace ultralight

    
