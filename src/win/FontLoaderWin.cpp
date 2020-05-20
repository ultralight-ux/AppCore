#include "FontLoaderWin.h"
#include <dwrite_3.h>
#include <wrl.h>
#include <wrl/client.h>
#include <MLang.h>
#include <memory>
#include "TextAnalysisSource.h"

using namespace Microsoft::WRL;

static DWRITE_FONT_WEIGHT toDWriteFontWeight(LONG fontWeight)
{
  if (fontWeight < LONG(150))
    return DWRITE_FONT_WEIGHT_THIN;
  if (fontWeight < LONG(250))
    return DWRITE_FONT_WEIGHT_EXTRA_LIGHT;
  if (fontWeight < LONG(350))
    return DWRITE_FONT_WEIGHT_LIGHT;
  if (fontWeight < LONG(400))
    return DWRITE_FONT_WEIGHT_SEMI_LIGHT;
  if (fontWeight < LONG(450))
    return DWRITE_FONT_WEIGHT_NORMAL;
  if (fontWeight < LONG(550))
    return DWRITE_FONT_WEIGHT_MEDIUM;
  if (fontWeight < LONG(650))
    return DWRITE_FONT_WEIGHT_SEMI_BOLD;
  if (fontWeight < LONG(750))
    return DWRITE_FONT_WEIGHT_BOLD;
  if (fontWeight < LONG(850))
    return DWRITE_FONT_WEIGHT_EXTRA_BOLD;
  if (fontWeight < LONG(950))
    return DWRITE_FONT_WEIGHT_BLACK;
  return DWRITE_FONT_WEIGHT_EXTRA_BLACK;
}

namespace ultralight {

String16 FontLoaderWin::fallback_font() const {
  return "Arial";
}

String16 FontLoaderWin::fallback_font_for_characters(const String16& characters,
  int weight, bool italic) const {
  ComPtr<IDWriteFactory2> pDWriteFactory;

  HRESULT hr = DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED,
    __uuidof(IDWriteFactory2),
    &pDWriteFactory
  );

  if (FAILED(hr)) return fallback_font();

  wchar_t localeName[LOCALE_NAME_MAX_LENGTH];
  int success = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);
  if (!success) return fallback_font();

  ComPtr<IDWriteFontFallback> pFontFallback;
  hr = pDWriteFactory->GetSystemFontFallback(&pFontFallback);

  if (FAILED(hr)) return fallback_font();

  TextAnalysisSource source(characters.data(), (UINT32)characters.length(), localeName, DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);

  UINT32 mappedLength;
  ComPtr<IDWriteFont> pMappedFont;
  FLOAT fontScale = 1.0f;
  hr = pFontFallback->MapCharacters(&source, 0, (UINT32)characters.length(), nullptr, nullptr,
    toDWriteFontWeight(weight), italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,
    DWRITE_FONT_STRETCH_NORMAL, &mappedLength, &pMappedFont, &fontScale);

  if (FAILED(hr) || !pMappedFont) return fallback_font();

  ComPtr<IDWriteFontFamily> pMappedFontFamily;
  hr = pMappedFont->GetFontFamily(&pMappedFontFamily);

  if (FAILED(hr)) return fallback_font();

  IDWriteLocalizedStrings* pFamilyNames = NULL;

  hr = pMappedFontFamily->GetFamilyNames(&pFamilyNames);

  if (FAILED(hr)) return fallback_font();

  UINT32 index = 0;
  BOOL exists = false;
  hr = pFamilyNames->FindLocaleName(localeName, &index, &exists);

  if (SUCCEEDED(hr) && !exists) {
    // If we didn't find a match, try again with US English
    hr = pFamilyNames->FindLocaleName(L"en-us", &index, &exists);
  }

  // If we still can't find matching localized name, just select first in list
  if (!exists)
    index = 0;

  UINT32 nameLength = 0;
  hr = pFamilyNames->GetStringLength(index, &nameLength);

  if (FAILED(hr) || !nameLength) return fallback_font();

  std::unique_ptr<wchar_t[]> name(new wchar_t[nameLength + 1]);
  hr = pFamilyNames->GetString(index, name.get(), nameLength + 1);

  if (FAILED(hr)) return fallback_font();

  return String16((const ultralight::Char16*)name.get(), nameLength);
}

static RefPtr<FontFile> LoadFont(const String16& family, int weight, bool italic) {
  ComPtr<IDWriteFactory> pDWriteFactory;

  HRESULT hr = DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED,
    __uuidof(IDWriteFactory),
    &pDWriteFactory
  );

  if (FAILED(hr)) return nullptr;

  // Get the system font collection.
  ComPtr<IDWriteFontCollection> pFontCollection;
  hr = pDWriteFactory->GetSystemFontCollection(&pFontCollection);

  if (FAILED(hr)) return nullptr;
  
  UINT32 index = 0;
  BOOL exists = false;
  size_t len = family.length() + 1;
  WCHAR* family_name_wcstr = new WCHAR[len];
  memset(family_name_wcstr, 0, sizeof(WCHAR) * len);
  memcpy(family_name_wcstr, family.data(), sizeof(WCHAR) * (len - 1));
  hr = pFontCollection->FindFamilyName(family_name_wcstr, &index, &exists);

  delete[] family_name_wcstr;

  if (FAILED(hr) || !exists) return nullptr;

  // Get the specified font family
  ComPtr<IDWriteFontFamily> pFontFamily;
  hr = pFontCollection->GetFontFamily(index, &pFontFamily);

  if (FAILED(hr) || pFontFamily->GetFontCount() == 0) return nullptr;

  // Get the first matching font
  ComPtr<IDWriteFont> pFont;
  hr = pFontFamily->GetFirstMatchingFont(toDWriteFontWeight(weight), DWRITE_FONT_STRETCH_NORMAL, 
    italic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL, &pFont);

  if (FAILED(hr)) return nullptr;

  // Create font face from the font
  ComPtr<IDWriteFontFace> pFontFace;
  hr = pFont->CreateFontFace(&pFontFace);

  if (FAILED(hr)) return nullptr;

  UINT32 numFiles = 0;
  hr = pFontFace->GetFiles(&numFiles, nullptr);

  if (FAILED(hr) || numFiles == 0) return nullptr;

  // We only support fonts contained in a single file
  if (numFiles > 1) 
    return nullptr;

  ComPtr<IDWriteFontFile> pFontFile;
  hr = pFontFace->GetFiles(&numFiles, &pFontFile);

  if (FAILED(hr)) return nullptr;

  BOOL isSupportedFontType = false;
  DWRITE_FONT_FILE_TYPE fontFileType;
  DWRITE_FONT_FACE_TYPE fontFaceType;
  UINT32 numberOfFaces;
  hr = pFontFile->Analyze(&isSupportedFontType, &fontFileType, &fontFaceType, &numberOfFaces);

  if (FAILED(hr) || !isSupportedFontType) 
    return nullptr;

  const void* referenceKey;
  UINT32 refKeySize;
  hr = pFontFile->GetReferenceKey(&referenceKey, &refKeySize);

  if (FAILED(hr)) return nullptr;

  ComPtr<IDWriteFontFileLoader> pFontFileLoader;
  hr = pFontFile->GetLoader(&pFontFileLoader);

  if (FAILED(hr)) return nullptr;

  // Check if this is a local font file, if so we will just pass back the filepath
  // to avoid loading the file into memory.
  IDWriteLocalFontFileLoader* localFileLoader = NULL;
  hr = pFontFileLoader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void**)&localFileLoader);
  if (SUCCEEDED(hr)) {
    UINT32 pathLength = 0;
    hr = localFileLoader->GetFilePathLengthFromKey(referenceKey, refKeySize, &pathLength);

    if (FAILED(hr)) return nullptr;

    WCHAR* path = new WCHAR[pathLength + 1];

    hr = localFileLoader->GetFilePathFromKey(referenceKey, refKeySize, path, pathLength + 1);
    if (FAILED(hr)) return nullptr;

    ultralight::String16 pathStr(path, pathLength);
    return FontFile::Create(pathStr);
  }

  ComPtr<IDWriteFontFileStream> pFontFileStream;
  hr = pFontFileLoader->CreateStreamFromKey(referenceKey, refKeySize, &pFontFileStream);

  if (FAILED(hr)) return nullptr;

  UINT64 fileSize = 0;
  hr = pFontFileStream->GetFileSize(&fileSize);

  if (FAILED(hr)) return nullptr;

  const void* fragmentStart;
  void* context;
  hr = pFontFileStream->ReadFileFragment(&fragmentStart, 0, fileSize, &context);

  if (FAILED(hr)) return nullptr;

  Ref<Buffer> result = Buffer::Create(fragmentStart, (size_t)fileSize);

  pFontFileStream->ReleaseFileFragment(context);

  return FontFile::Create(result);
}

RefPtr<FontFile> FontLoaderWin::Load(const String16& family, int weight, bool italic) {
  return LoadFont(family, weight, italic);
}

}  // namespace ultralight
