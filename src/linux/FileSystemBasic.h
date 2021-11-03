#pragma once
#include <Ultralight/platform/FileSystem.h>
#include <string>
#include <map>
#include <fstream>
#include <memory>

namespace ultralight {

/**
 * Basic FileSystem interface, implemented using the C++ STL.
 */
class FileSystemBasic : public FileSystem {
 public:
    FileSystemBasic(const char* baseDir);
    virtual ~FileSystemBasic() {}
    
    virtual bool FileExists(const String& path) override;

    virtual bool GetFileSize(FileHandle handle, int64_t& result) override;

    virtual bool GetFileMimeType(const String& path, String& result) override;

    virtual FileHandle OpenFile(const String& path, bool open_for_writing) override;

    virtual void CloseFile(FileHandle& handle) override;

    virtual int64_t ReadFromFile(FileHandle handle, char* data, int64_t length) override;

protected:
    std::string baseDir_;
    std::string getRelative(const String& path);
    FileHandle next_handle_ = 0;
    std::map<FileHandle, std::unique_ptr<std::ifstream>> open_files_;
};
    
}  // namespace ultralight

