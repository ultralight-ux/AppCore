/*
* Copryight (C) 2017 Ultralight, Inc. All rights reserved.
* Copyright (C) 2007-2017 Apple Inc. All rights reserved.
* Copyright (C) 2008 Collabora, Ltd. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1.  Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
* 2.  Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
* 3.  Neither the name of Apple Inc. ("Apple") nor the names of
*     its contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "FileSystemWin.h"
#include <io.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <sys/stat.h>
#include <windows.h>
#include <limits>
#include <stdio.h>
#include <Ultralight/String.h>
#include <string>
#include <algorithm>
#include <memory>
#include <Strsafe.h>

namespace ultralight {

static bool getFindData(LPCWSTR path, WIN32_FIND_DATAW& findData)
{
  HANDLE handle = FindFirstFileW(path, &findData);
  if (handle == INVALID_HANDLE_VALUE)
    return false;
  FindClose(handle);
  return true;
}

static bool getFileSizeFromByHandleFileInformationStructure(const BY_HANDLE_FILE_INFORMATION& fileInformation, int64_t& size)
{
  ULARGE_INTEGER fileSize;
  fileSize.HighPart = fileInformation.nFileSizeHigh;
  fileSize.LowPart = fileInformation.nFileSizeLow;

  if (fileSize.QuadPart > static_cast<ULONGLONG>(std::numeric_limits<int64_t>::max()))
    return false;

  size = fileSize.QuadPart;
  return true;
}

bool getFileSize(FileHandle fileHandle, int64_t& size)
{
  BY_HANDLE_FILE_INFORMATION fileInformation;
  if (!::GetFileInformationByHandle((HANDLE)fileHandle, &fileInformation))
    return false;

  return getFileSizeFromByHandleFileInformationStructure(fileInformation, size);
}

std::wstring GetMimeType(const std::wstring &szExtension)
{
  // return mime type for extension
  HKEY hKey = NULL;
  std::wstring szResult = L"application/unknown";

  // open registry key
  if (RegOpenKeyExW(HKEY_CLASSES_ROOT, szExtension.c_str(),
    0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    // define buffer
    wchar_t szBuffer[256] = { 0 };
    DWORD dwBuffSize = sizeof(szBuffer);

    // get content type
    if (RegQueryValueExW(hKey, L"Content Type", NULL, NULL,
      (LPBYTE)szBuffer, &dwBuffSize) == ERROR_SUCCESS)
    {
      // success
      szResult = szBuffer;
    }

    // close key
    RegCloseKey(hKey);
  }

  // return result
  return szResult;
}

FileSystemWin::FileSystemWin(LPCWSTR baseDir) {
  baseDir_.reset(new WCHAR[_MAX_PATH]);
  StringCchCopyW(baseDir_.get(), MAX_PATH, baseDir);
}

FileSystemWin::~FileSystemWin() {}

bool FileSystemWin::FileExists(const String16& path) {
  WIN32_FIND_DATAW findData;
  return getFindData(GetRelative(path).get(), findData);
}

bool FileSystemWin::GetFileMimeType(const String16& path, String16& result)
{
  LPWSTR ext = PathFindExtensionW(path.data());
  std::wstring mimetype = GetMimeType(ext);
  result = String16(mimetype.c_str(), mimetype.length());
  return true;
}

bool FileSystemWin::GetFileSize(FileHandle handle, int64_t& result) {
  BY_HANDLE_FILE_INFORMATION fileInformation;
  if (!::GetFileInformationByHandle((HANDLE)handle, &fileInformation))
    return false;

  return getFileSizeFromByHandleFileInformationStructure(fileInformation, result);
}

FileHandle FileSystemWin::OpenFile(const String16& path, bool open_for_writing) {
  DWORD desiredAccess = 0;
  DWORD creationDisposition = 0;
  DWORD shareMode = 0;
  if (open_for_writing) {
    desiredAccess = GENERIC_WRITE;
    creationDisposition = CREATE_ALWAYS;
  } else {
    desiredAccess = GENERIC_READ;
    creationDisposition = OPEN_EXISTING;
    shareMode = FILE_SHARE_READ;
  }

  return (FileHandle)CreateFileW(GetRelative(path).get(), desiredAccess, shareMode, 
    0, creationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
}

void FileSystemWin::CloseFile(FileHandle& handle) {
  if (handle != (FileHandle)INVALID_HANDLE_VALUE) {
    ::CloseHandle((HANDLE)handle);
    handle = (FileHandle)INVALID_HANDLE_VALUE;
  }
}

int64_t FileSystemWin::ReadFromFile(FileHandle handle, char* data, int64_t length) {
  if (handle == (FileHandle)INVALID_HANDLE_VALUE)
    return -1;

  DWORD bytesRead;
  bool success = !!::ReadFile((HANDLE)handle, data, (DWORD)length, &bytesRead, 0);

  if (!success)
    return -1;
  return static_cast<int64_t>(bytesRead);
}

std::unique_ptr<WCHAR[]> FileSystemWin::GetRelative(const String16& path) {
  std::unique_ptr<WCHAR[]> relPath(new WCHAR[_MAX_PATH]);
  memset(relPath.get(), 0, _MAX_PATH * sizeof(WCHAR));
  PathCombineW(relPath.get(), baseDir_.get(), path.data());
  return relPath;
}
  
// Called from Platform.cpp
ultralight::FileSystem* CreatePlatformFileSystem(const String& baseDir) {
  std::wstring baseDirWStr(baseDir.data());
  
  WCHAR cur_dir[_MAX_PATH];
  GetCurrentDirectoryW(_MAX_PATH, cur_dir);
  WCHAR absolute_dir[_MAX_PATH];
  PathCombineW(absolute_dir, cur_dir, baseDirWStr.c_str());
  
  return new ultralight::FileSystemWin(absolute_dir);
}

}  // namespace ultralight
