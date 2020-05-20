#pragma once
#include <dwrite_3.h>

class TextAnalysisSource
  : public IDWriteTextAnalysisSource
{
public:
  // IUnknown interface
  IFACEMETHOD(QueryInterface)(IID const& iid, OUT void** ppObject)
  {
    if (iid == __uuidof(IDWriteTextAnalysisSource)) {
      *ppObject = static_cast<IDWriteTextAnalysisSource*>(this);
      return S_OK;
    }
    else if (iid == __uuidof(IUnknown)) {
      *ppObject =
        static_cast<IUnknown*>(static_cast<IDWriteTextAnalysisSource*>(this));
      return S_OK;
    }
    else {
      return E_NOINTERFACE;
    }
  }

  IFACEMETHOD_(ULONG, AddRef)()
  {
    return InterlockedIncrement(&mRefValue);
  }

  IFACEMETHOD_(ULONG, Release)()
  {
    ULONG newCount = InterlockedDecrement(&mRefValue);
    if (newCount == 0)
      delete this;

    return newCount;
  }

public:
  TextAnalysisSource(const wchar_t* text,
    UINT32 textLength,
    const wchar_t* localeName,
    DWRITE_READING_DIRECTION readingDirection);

  ~TextAnalysisSource();

  // IDWriteTextAnalysisSource implementation
  IFACEMETHODIMP GetTextAtPosition(UINT32 textPosition,
    OUT WCHAR const** textString,
    OUT UINT32* textLength);

  IFACEMETHODIMP GetTextBeforePosition(UINT32 textPosition,
    OUT WCHAR const** textString,
    OUT UINT32* textLength);

  IFACEMETHODIMP_(DWRITE_READING_DIRECTION)
    GetParagraphReadingDirection() throw();

  IFACEMETHODIMP GetLocaleName(UINT32 textPosition,
    OUT UINT32* textLength,
    OUT WCHAR const** localeName);

  IFACEMETHODIMP
    GetNumberSubstitution(UINT32 textPosition,
      OUT UINT32* textLength,
      OUT IDWriteNumberSubstitution** numberSubstitution);

protected:
  UINT32 mTextLength;
  const wchar_t* mText;
  const wchar_t* mLocaleName;
  DWRITE_READING_DIRECTION mReadingDirection;
  ULONG mRefValue;

private:
  // No copy construction allowed.
  TextAnalysisSource(const TextAnalysisSource& b) = delete;
  TextAnalysisSource& operator=(TextAnalysisSource const&) = delete;
};
