#pragma once
#include <Ultralight/platform/FileSystem.h>
#include <string>

namespace ultralight {

/**
 * Basic FileSystem interface, implemented using the C++ STL.
 */
class FileSystemBasic : public FileSystem {
 public:
    FileSystemBasic(const char* baseDir);
    virtual ~FileSystemBasic() {}
    
    virtual bool FileExists(const String& path) override;

    virtual String GetFileMimeType(const String& file_path) override;

    virtual String GetFileCharset(const String& file_path) override;

    virtual RefPtr<Buffer> OpenFile(const String& file_path) override;

protected:
    std::string baseDir_;
    std::string getRelative(const String& path);
};
    
}  // namespace ultralight

