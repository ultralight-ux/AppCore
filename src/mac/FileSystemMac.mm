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

bool FileSystemMac::GetFileSize(FileHandle handle, int64_t& result) {
    struct stat fileInfo;
    if (fstat((int)handle, &fileInfo))
        return false;
    
    result = fileInfo.st_size;
    return true;
}

bool FileSystemMac::GetFileMimeType(const String& path, String& result) {
    auto pathStr = ToNSString(getRelative(path.utf16()));
    CFStringRef extension = (__bridge CFStringRef)[pathStr pathExtension];
    CFStringRef mime_type = NULL;
    CFStringRef identifier = UTTypeCreatePreferredIdentifierForTag(kUTTagClassFilenameExtension, extension, NULL);
    if (identifier)
        mime_type = UTTypeCopyPreferredTagWithClass(identifier, kUTTagClassMIMEType);
    CFRelease(identifier);
    result = ToString16(mime_type);
    return !result.empty();
}

FileHandle FileSystemMac::OpenFile(const String& path, bool open_for_writing) {
    std::string fsRep = fileSystemRepresentation(getRelative(path.utf16()));
    
    if (fsRep.empty())
        return invalidFileHandle;
    
    int platformFlag = 0;
    platformFlag |= open_for_writing? (O_WRONLY | O_CREAT | O_TRUNC) : O_RDONLY;
    
    int handle = open(fsRep.data(), platformFlag, 0666);
    
    return handle;
}

void FileSystemMac::CloseFile(FileHandle& handle) {
    if (handle != invalidFileHandle)
        close((int)handle);
    handle = invalidFileHandle;
}

int64_t FileSystemMac::ReadFromFile(FileHandle handle, char* data, int64_t length) {
    do {
        auto bytesRead = read((int)handle, data, static_cast<size_t>(length));
        if (bytesRead >= 0)
            return bytesRead;
    } while (errno == EINTR);
    return -1;
}

String16 FileSystemMac::getRelative(const ultralight::String16 &path) {
    return GetPathByAppendingComponent(base_dir_, path);
}

// Called from Platform.cpp
FileSystem* CreatePlatformFileSystem(const String& baseDir) {
    return new FileSystemMac(baseDir.utf16());
}
    
} // namespace ultralight

