#import "FileSystemMac.h"
#import <CoreFoundation/CoreFoundation.h>
#import <CoreServices/CoreServices.h>
#import <Foundation/Foundation.h>
#include <CoreFoundation/CFString.h>
#include <Ultralight/String.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <libgen.h>
#include <fnmatch.h>
#include <dirent.h>

namespace ultralight {
    
static NSString* ToNSString(const String16& str) {
    auto pathData = [[NSData alloc] initWithBytesNoCopy:const_cast<void*>(reinterpret_cast<const void*>(str.data()))
                                                 length:str.length() * sizeof(Char16)
                                           freeWhenDone:NO];
    return [[NSString alloc] initWithData:pathData
                                 encoding:NSUTF16LittleEndianStringEncoding];
}
  
static String16 GetPathByAppendingComponent(const String16& path, const String16& component) {
  if (path.empty())
    return component;
  
  String16 suffix = component;
  if (suffix.empty())
    return path;
  
  // Strip slash prefix from suffix
  if (suffix.data()[0] == (Char16)'/')
    suffix = String16(suffix.data() + 1, suffix.length() - 1);
  
  // Length includes terminating null character so gotta subtract 2
  Char16 lastChar = path.data()[path.length() - 2];
  if (lastChar == (Char16)'/')
    return path + suffix;
  
  const char* pathSeparator = "/";
  return path + String(pathSeparator).utf16() + suffix;
}

std::string fileSystemRepresentation(const String16& str) {
    if (str.empty())
        return std::string();
    
    CFStringRef strCF = CFStringCreateWithCharacters(kCFAllocatorDefault, reinterpret_cast<const UniChar*>(str.data()), str.length());
    if (!strCF)
        return std::string();
    
    CFIndex size = CFStringGetMaximumSizeOfFileSystemRepresentation(strCF);
    std::vector<char> buffer(size);
    if (!CFStringGetFileSystemRepresentation(strCF, buffer.data(), buffer.size()))
        return std::string();
    
    return std::string(buffer.data(), strlen(buffer.data()));
}

String16 ToString16(CFStringRef str) {
    if (!str)
        return String16();
    CFIndex size = CFStringGetLength(str);
    std::vector<Char16> buffer(size);
    CFStringGetCharacters(str, CFRangeMake(0, size), (UniChar*)buffer.data());
    return String16(buffer.data(), size);
}

String16 ToString16(const std::string& utf8) {
    if (utf8.empty())
        return String16();
    return String(utf8.data(), utf8.length()).utf16();
}
    
static bool Equals(const String16& a, const String16& b) {
    if (a.length() != b.length())
        return false;
    
    for (size_t i = 0; i < a.length(); ++i)
        if (a.data()[i] != b.data()[i])
            return false;
    
    return true;
}

FileSystemMac::FileSystemMac(const String& baseDir) : base_dir_(baseDir.utf16()) {
    if (Equals(base_dir_, String("@resource_path").utf16()))
        base_dir_ = ToString16((__bridge CFStringRef)[[NSBundle mainBundle] resourcePath]);
}
    
FileSystemMac::~FileSystemMac() {}

bool FileSystemMac::FileExists(const String& path) {
    if (path.empty())
        return false;
    
    std::string fsRep = fileSystemRepresentation(getRelative(path.utf16()));
    
    if (!fsRep.data() || fsRep.data()[0] == '\0')
        return false;
    
    return access(fsRep.data(), F_OK) != -1;
}

String FileSystemMac::GetFileMimeType(const String& file_path) {
    auto pathStr = ToNSString(getRelative(file_path.utf16()));
    CFStringRef extension = (__bridge CFStringRef)[pathStr pathExtension];
    CFStringRef mime_type = NULL;
    CFStringRef identifier = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, extension, NULL);
    if (identifier)
        mime_type = UTTypeCopyPreferredTagWithClass(identifier, kUTTagClassMIMEType);
    CFRelease(identifier);
    return ToString16(mime_type);
}

String FileSystemMac::GetFileCharset(const String& file_path) {
    return "utf-8";
}

struct FileSystemMac_BufferData {
  int file_handle;
  size_t file_size;
  void* file_map;
};

void FileSystemMac_DestroyBufferCallback(void* user_data, void* data) {
  FileSystemMac_BufferData* buffer_data = reinterpret_cast<FileSystemMac_BufferData*>(user_data);
  munmap(buffer_data->file_map, buffer_data->file_size);
  close(buffer_data->file_handle);
  delete buffer_data;
}

RefPtr<Buffer> FileSystemMac::OpenFile(const String& file_path) {
    std::string fsRep = fileSystemRepresentation(getRelative(file_path.utf16()));
    
    if (fsRep.empty())
        return nullptr;

    int file_handle = open(fsRep.data(), O_RDONLY);

    if (file_handle == -1) {
        // Error: Could not open file for reading
        return nullptr;
    }

    struct stat file_info = {0};
    
    if (fstat(file_handle, &file_info) == -1) {
        // Error: Could not get the file size
        close(file_handle);
        return nullptr;
    }

    size_t file_size = file_info.st_size;

    if (file_size == 0) {
        // Error: File is empty
        close(file_handle);
        return nullptr;
    }

    void* file_map = mmap(0, file_size, PROT_READ, MAP_SHARED, file_handle, 0);

    if (file_map == MAP_FAILED) {
        // Error: File mapping failed
        close(file_handle);
        return nullptr;
    }

    FileSystemMac_BufferData* buffer_data = new FileSystemMac_BufferData();
    buffer_data->file_handle = file_handle;
    buffer_data->file_size = file_size;
    buffer_data->file_map = file_map;

    return Buffer::Create(file_map, file_size, buffer_data, FileSystemMac_DestroyBufferCallback);
}

String16 FileSystemMac::getRelative(const ultralight::String16 &path) {
    return GetPathByAppendingComponent(base_dir_, path);
}

// Called from Platform.cpp
FileSystem* CreatePlatformFileSystem(const String& baseDir) {
    return new FileSystemMac(baseDir.utf16());
}
    
} // namespace ultralight

