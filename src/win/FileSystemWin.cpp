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
#include <Ultralight/Buffer.h>
#include <string>
#include <algorithm>
#include <memory>
#include <Strsafe.h>

namespace ultralight {

static bool getFindData(LPCWSTR path, WIN32_FIND_DATAW& findData) {
  HANDLE handle = FindFirstFileW(path, &findData);
  if (handle == INVALID_HANDLE_VALUE)
    return false;
  FindClose(handle);
  return true;
}

std::wstring GetMimeType(const std::wstring& szExtension) {
  // return mime type for extension
  HKEY hKey = NULL;
  std::wstring szResult = L"application/unknown";

  // open registry key
  if (RegOpenKeyExW(HKEY_CLASSES_ROOT, szExtension.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
    // define buffer
    wchar_t szBuffer[256] = { 0 };
    DWORD dwBuffSize = sizeof(szBuffer);

    // get content type
    if (RegQueryValueExW(hKey, L"Content Type", NULL, NULL, (LPBYTE)szBuffer, &dwBuffSize)
        == ERROR_SUCCESS) {
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

FileSystemWin::~FileSystemWin() { }

bool FileSystemWin::FileExists(const String& path) {
  WIN32_FIND_DATAW findData;
  return getFindData(GetRelative(path).get(), findData);
}

String FileSystemWin::GetFileMimeType(const String& file_path) {
  String16 path16 = file_path.utf16();
  LPWSTR ext = PathFindExtensionW(path16.data());
  std::wstring mimetype = GetMimeType(ext);
  return String16(mimetype.c_str(), mimetype.length());
}

String FileSystemWin::GetFileCharset(const String& file_path) { return "utf-8"; }

struct FileSystemWin_BufferData {
  HANDLE hFile;
  HANDLE hMap;
  LPVOID lpBasePtr;
};

void FileSystemWin_DestroyBufferCallback(void* user_data, void* data) {
  FileSystemWin_BufferData* buffer_data = reinterpret_cast<FileSystemWin_BufferData*>(user_data);
  UnmapViewOfFile(buffer_data->lpBasePtr);
  CloseHandle(buffer_data->hMap);
  CloseHandle(buffer_data->hFile);
  delete buffer_data;
}

RefPtr<Buffer> FileSystemWin::OpenFile(const String& file_path) {
  auto pathStr = GetRelative(file_path);
  HANDLE hFile;
  HANDLE hMap;
  LPVOID lpBasePtr;
  LARGE_INTEGER liFileSize;

  hFile = CreateFile(pathStr.get(),
                     GENERIC_READ,          // dwDesiredAccess
                     FILE_SHARE_READ,       // dwShareMode
                     NULL,                  // lpSecurityAttributes
                     OPEN_EXISTING,         // dwCreationDisposition
                     FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
                     0);                    // hTemplateFile

  if (hFile == INVALID_HANDLE_VALUE) {
    return nullptr;
  }

  if (!GetFileSizeEx(hFile, &liFileSize)) {
    CloseHandle(hFile);
    return nullptr;
  }

  if (liFileSize.QuadPart == 0) {
    CloseHandle(hFile);
    return nullptr;
  }

  hMap = CreateFileMapping(hFile,
                           NULL,          // Mapping attributes
                           PAGE_READONLY, // Protection flags
                           0,             // MaximumSizeHigh
                           0,             // MaximumSizeLow
                           NULL);         // Name

  if (hMap == 0) {
    CloseHandle(hFile);
    return nullptr;
  }

  lpBasePtr = MapViewOfFile(hMap,
                            FILE_MAP_READ, // dwDesiredAccess
                            0,             // dwFileOffsetHigh
                            0,             // dwFileOffsetLow
                            0);            // dwNumberOfBytesToMap

  if (lpBasePtr == NULL) {
    CloseHandle(hMap);
    CloseHandle(hFile);
    return nullptr;
  }

  FileSystemWin_BufferData* buffer_data = new FileSystemWin_BufferData();
  buffer_data->hFile = hFile;
  buffer_data->hMap = hMap;
  buffer_data->lpBasePtr = lpBasePtr;

  return Buffer::Create((char*)lpBasePtr, (size_t)liFileSize.QuadPart, buffer_data,
                        FileSystemWin_DestroyBufferCallback);
}

std::unique_ptr<WCHAR[]> FileSystemWin::GetRelative(const String& path) {
  String16 path16 = path.utf16();
  std::unique_ptr<WCHAR[]> relPath(new WCHAR[_MAX_PATH]);
  memset(relPath.get(), 0, _MAX_PATH * sizeof(WCHAR));
  PathCombineW(relPath.get(), baseDir_.get(), path16.data());
  return relPath;
}

// Called from Platform.cpp
ultralight::FileSystem* CreatePlatformFileSystem(const String& baseDir) {
  std::wstring baseDirWStr(baseDir.utf16().data());

  WCHAR cur_dir[_MAX_PATH];
  GetCurrentDirectoryW(_MAX_PATH, cur_dir);
  WCHAR absolute_dir[_MAX_PATH];
  PathCombineW(absolute_dir, cur_dir, baseDirWStr.c_str());

  return new ultralight::FileSystemWin(absolute_dir);
}

} // namespace ultralight
