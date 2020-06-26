#include "FontLoaderLinux.h"
#include <fontconfig/fontconfig.h>
#include <memory>
#include <map>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>

namespace ultralight {

FontLoaderLinux::FontLoaderLinux() {
    FcInit();
}

FontLoaderLinux::~FontLoaderLinux() {}

template<typename T>
struct FcPtrDeleter {
    void operator()(T* ptr) const = delete;
};

template<typename T>
using FcUniquePtr = std::unique_ptr<T, FcPtrDeleter<T>>;

template<> struct FcPtrDeleter<FcCharSet> { 
  void operator()(FcCharSet* ptr) const { FcCharSetDestroy(ptr); }
};

template<> struct FcPtrDeleter<FcFontSet> {
    void operator()(FcFontSet* ptr) const { FcFontSetDestroy(ptr); }
};

template<> struct FcPtrDeleter<FcLangSet> {
  void operator()(FcLangSet* ptr) const { FcLangSetDestroy(ptr); }
};

template<> struct FcPtrDeleter<FcObjectSet> {
  void operator()(FcObjectSet* ptr) const { FcObjectSetDestroy(ptr); }
};

template<> struct FcPtrDeleter<FcPattern> {
  void operator()(FcPattern* ptr) const { FcPatternDestroy(ptr); }
};

template<> struct FcPtrDeleter<FcConfig> {
  void operator()(FcConfig* ptr) const { FcConfigDestroy(ptr); }
};

static int fontWeightToFontconfigWeight(int weight)
{
    if (weight < (150))
        return FC_WEIGHT_THIN;
    if (weight < (250))
        return FC_WEIGHT_ULTRALIGHT;
    if (weight < (350))
        return FC_WEIGHT_LIGHT;
    if (weight < (450))
        return FC_WEIGHT_REGULAR;
    if (weight < (550))
        return FC_WEIGHT_MEDIUM;
    if (weight < (650))
        return FC_WEIGHT_SEMIBOLD;
    if (weight < (750))
        return FC_WEIGHT_BOLD;
    if (weight < (850))
        return FC_WEIGHT_EXTRABOLD;
    return FC_WEIGHT_ULTRABLACK;
}

String16 FontLoaderLinux::fallback_font() const { return "sans"; }

String16 FontLoaderLinux::fallback_font_for_characters(const String16& characters, int weight, bool italic) const {
  return fallback_font();
}

static bool configurePatternForFontDescription(FcPattern* pattern, int weight, bool italic, float size)
{
    if (!FcPatternAddInteger(pattern, FC_SLANT, italic ? FC_SLANT_ITALIC : FC_SLANT_ROMAN))
        return false;
    if (!FcPatternAddInteger(pattern, FC_WEIGHT, fontWeightToFontconfigWeight(weight)))
        return false;
    if (!FcPatternAddDouble(pattern, FC_PIXEL_SIZE, (double)size))
        return false;
    return true;
}

static inline bool EqualsIgnoringCase(const char * s1, const char * s2) {
  const unsigned char *p1 = (const unsigned char *) s1;
  const unsigned char *p2 = (const unsigned char *) s2;
  int result;
  if (p1 == p2)
    return true;
  while ((result = tolower (*p1) - tolower (*p2++)) == 0)
    if (*p1++ == '\0')
      break;
  return result == 0;
}

static inline std::string ToAscii(const String16& str) {
  std::string result;
  for (size_t i = 0; i < str.size(); i++) {
    if (str.data()[i] < 0x7F)
      result += static_cast<char>(str.data()[i]);
  }
  return result;
}

static inline bool isCommonlyUsedGenericFamily(const char* familyNameString)
{
  return EqualsIgnoringCase(familyNameString, "sans")
      || EqualsIgnoringCase(familyNameString, "sans-serif")
      || EqualsIgnoringCase(familyNameString, "serif")
      || EqualsIgnoringCase(familyNameString, "monospace")
      || EqualsIgnoringCase(familyNameString, "fantasy")
      || EqualsIgnoringCase(familyNameString, "-webkit-system-font")
      || EqualsIgnoringCase(familyNameString, "-webkit-system-ui")
      || EqualsIgnoringCase(familyNameString, "cursive");
}

static inline std::vector<std::string> SplitString(const std::string& input, char delimiter) {
  std::stringstream stream(input);
  std::string token;
  std::vector<std::string> result;
  while(std::getline(stream, token, delimiter))
    result.push_back(token);

  return result;
}

static std::vector<std::string> patternToFamilies(FcPattern& pattern) {
  char* patternChars = reinterpret_cast<char*>(FcPatternFormat(&pattern, reinterpret_cast<const FcChar8*>("%{family}")));
  std::string patternString(patternChars);
  free(patternChars);

  return SplitString(patternString, ',');
}

// This is based on Chromium BSD code from Skia (src/ports/SkFontMgr_fontconfig.cpp). It is a
// hack for lack of API in Fontconfig: https://bugs.freedesktop.org/show_bug.cgi?id=19375
// FIXME: This is horrible. It should be deleted once Fontconfig can do this itself.
enum class AliasStrength {
    Weak,
    Strong,
    Done
};

static AliasStrength strengthOfFirstAlias(const FcPattern& original)
{
    // Ideally there would exist a call like
    // FcResult FcPatternIsWeak(pattern, object, id, FcBool* isWeak);
    //
    // However, there is no such call and as of Fc 2.11.0 even FcPatternEquals ignores the weak bit.
    // Currently, the only reliable way of finding the weak bit is by its effect on matching.
    // The weak bit only affects the matching of FC_FAMILY and FC_POSTSCRIPT_NAME object values.
    // A element with the weak bit is scored after FC_LANG, without the weak bit is scored before.
    // Note that the weak bit is stored on the element, not on the value it holds.
    FcValue value;
    FcResult result = FcPatternGet(&original, FC_FAMILY, 0, &value);
    if (result != FcResultMatch)
        return AliasStrength::Done;

    FcUniquePtr<FcPattern> pattern(FcPatternDuplicate(&original));
    FcBool hasMultipleFamilies = true;
    while (hasMultipleFamilies)
        hasMultipleFamilies = FcPatternRemove(pattern.get(), FC_FAMILY, 1);

    // Create a font set with two patterns.
    // 1. the same FC_FAMILY as pattern and a lang object with only 'nomatchlang'.
    // 2. a different FC_FAMILY from pattern and a lang object with only 'matchlang'.
    FcUniquePtr<FcFontSet> fontSet(FcFontSetCreate());

    FcUniquePtr<FcLangSet> strongLangSet(FcLangSetCreate());
    FcLangSetAdd(strongLangSet.get(), reinterpret_cast<const FcChar8*>("nomatchlang"));
    // Ownership of this FcPattern will be transferred with FcFontSetAdd.
    FcPattern* strong = FcPatternDuplicate(pattern.get());
    FcPatternAddLangSet(strong, FC_LANG, strongLangSet.get());

    FcUniquePtr<FcLangSet> weakLangSet(FcLangSetCreate());
    FcLangSetAdd(weakLangSet.get(), reinterpret_cast<const FcChar8*>("matchlang"));
    // Ownership of this FcPattern will be transferred via FcFontSetAdd.
    FcPattern* weak = FcPatternCreate();
    FcPatternAddString(weak, FC_FAMILY, reinterpret_cast<const FcChar8*>("nomatchstring"));
    FcPatternAddLangSet(weak, FC_LANG, weakLangSet.get());

    FcFontSetAdd(fontSet.get(), strong);
    FcFontSetAdd(fontSet.get(), weak);

    // Add 'matchlang' to the copy of the pattern.
    FcPatternAddLangSet(pattern.get(), FC_LANG, weakLangSet.get());

    // Run a match against the copy of the pattern.
    // If the first element was weak, then we should match the pattern with 'matchlang'.
    // If the first element was strong, then we should match the pattern with 'nomatchlang'.

    // Note that this config is only used for FcFontRenderPrepare, which we don't even want.
    // However, there appears to be no way to match/sort without it.
    FcUniquePtr<FcConfig> config(FcConfigCreate());
    FcFontSet* fontSets[1] = { fontSet.get() };
    FcUniquePtr<FcPattern> match(FcFontSetMatch(config.get(), fontSets, 1, pattern.get(), &result));

    FcLangSet* matchLangSet;
    FcPatternGetLangSet(match.get(), FC_LANG, 0, &matchLangSet);
    return FcLangEqual == FcLangSetHasLang(matchLangSet, reinterpret_cast<const FcChar8*>("matchlang"))
        ? AliasStrength::Weak : AliasStrength::Strong;
}

static std::vector<std::string> strongAliasesForFamily(const char* family)
{
    FcUniquePtr<FcPattern> pattern(FcPatternCreate());
    if (!FcPatternAddString(pattern.get(), FC_FAMILY, reinterpret_cast<const FcChar8*>(family)))
        return std::vector<std::string>();

    FcConfigSubstitute(nullptr, pattern.get(), FcMatchPattern);
    FcDefaultSubstitute(pattern.get());

    FcUniquePtr<FcObjectSet> familiesOnly(FcObjectSetBuild(FC_FAMILY, nullptr));
    FcUniquePtr<FcPattern> minimal(FcPatternFilter(pattern.get(), familiesOnly.get()));

    // We really want to match strong (preferred) and same (acceptable) only here.
    // If a family name was specified, assume that any weak matches after the last strong match
    // are weak (default) and ignore them.
    // The reason for is that after substitution the pattern for 'sans-serif' looks like
    // "wwwwwwwwwwwwwwswww" where there are many weak but preferred names, followed by defaults.
    // So it is possible to have weakly matching but preferred names.
    // In aliases, bindings are weak by default, so this is easy and common.
    // If no family name was specified, we'll probably only get weak matches, but that's ok.
    int lastStrongId = -1;
    int numIds = 0;
    for (int id = 0; ; ++id) {
        AliasStrength result = strengthOfFirstAlias(*minimal);
        if (result == AliasStrength::Done) {
            numIds = id;
            break;
        }
        if (result == AliasStrength::Strong)
            lastStrongId = id;
        if (!FcPatternRemove(minimal.get(), FC_FAMILY, 0))
            return std::vector<std::string>();
    }

    // If they were all weak, then leave the pattern alone.
    if (lastStrongId < 0)
        return std::vector<std::string>();

    // Remove everything after the last strong.
    for (int id = lastStrongId + 1; id < numIds; ++id) {
        if (!FcPatternRemove(pattern.get(), FC_FAMILY, lastStrongId + 1)) {
            //ASSERT_NOT_REACHED();
            return std::vector<std::string>();
        }
    }

    return patternToFamilies(*pattern);
}

static bool areStronglyAliased(const char* familyA, const char* familyB)
{
    for (auto& family : strongAliasesForFamily(familyA)) {
        if (EqualsIgnoringCase(family.c_str(), familyB))
            return true;
    }
    return false;
}

FcUniquePtr<FcPattern> GetPatternForDescription(const std::string& family, int weight, bool italic, float size) {
  FcInit();
  
  // The CSS font matching algorithm (http://www.w3.org/TR/css3-fonts/#font-matching-algorithm)
  // says that we must find an exact match for font family, slant (italic or oblique can be used)
  // and font weight (we only match bold/non-bold here).
  FcUniquePtr<FcPattern> pattern(FcPatternCreate());
  // Never choose unscalable fonts, as they pixelate when displayed at different sizes.
  FcPatternAddBool(pattern.get(), FC_SCALABLE, FcTrue);

  // TODO: handle -webkit-system-font

  const char* familyNameString = family.c_str();
  if (!FcPatternAddString(pattern.get(), FC_FAMILY, reinterpret_cast<const FcChar8*>(familyNameString)))
      return nullptr;

  if (!configurePatternForFontDescription(pattern.get(), weight, italic, size))
      return nullptr;

  // The strategy is originally from Skia (src/ports/SkFontHost_fontconfig.cpp):
  //
  // We do not normally allow fontconfig to substitute one font family for another, since this
  // would break CSS font family fallback: the website should be in control of fallback. During
  // normal font matching, the only font family substitution permitted is for generic families
  // (sans, serif, monospace) or for strongly-aliased fonts (which are to be treated as
  // effectively identical). This is because the font matching step is designed to always find a
  // match for the font, which we don't want.
  //
  // Fontconfig is used in two stages: (1) configuration and (2) matching. During the
  // configuration step, before any matching occurs, we allow arbitrary family substitutions,
  // since this is an exact matter of respecting the user's font configuration.
  FcConfigSubstitute(nullptr, pattern.get(), FcMatchPattern);
  FcDefaultSubstitute(pattern.get());

  FcChar8* fontConfigFamilyNameAfterConfiguration;
  FcPatternGetString(pattern.get(), FC_FAMILY, 0, &fontConfigFamilyNameAfterConfiguration);
  const char* familyNameAfterConfiguration = reinterpret_cast<char*>(fontConfigFamilyNameAfterConfiguration);

  FcResult fontConfigResult;
  FcUniquePtr<FcPattern> resultPattern(FcFontMatch(nullptr, pattern.get(), &fontConfigResult));
  if (!resultPattern) // No match.
      return nullptr;

  // Loop through each font family of the result to see if it fits the one we requested.
  bool matchedFontFamily = false;
  FcChar8* fontConfigFamilyNameAfterMatching;
  for (int i = 0; FcPatternGetString(resultPattern.get(), FC_FAMILY, i, &fontConfigFamilyNameAfterMatching) == FcResultMatch; ++i) {
      // If Fontconfig gave us a different font family than the one we requested, we should ignore it
      // and allow WebCore to give us the next font on the CSS fallback list. The exceptions are if
      // this family name is a commonly-used generic family, or if the families are strongly-aliased.
      // Checking for a strong alias comes last, since it is slow.
      const char* familyNameAfterMatching = reinterpret_cast<char*>(fontConfigFamilyNameAfterMatching);
      if (EqualsIgnoringCase(familyNameAfterConfiguration, familyNameAfterMatching) || 
          isCommonlyUsedGenericFamily(familyNameString) || 
          areStronglyAliased(familyNameAfterConfiguration, familyNameAfterMatching)) {
          matchedFontFamily = true;
          break;
      }
  }

  if (!matchedFontFamily)
      return nullptr;

  // Verify that this font has an encoding compatible with Fontconfig. Fontconfig currently
  // supports three encodings in FcFreeTypeCharIndex: Unicode, Symbol and AppleRoman.
  // If this font doesn't have one of these three encodings, don't select it.
  //auto platformData = std::make_unique<FontPlatformData>(resultPattern.get(), fontDescription);
  //if (!platformData->hasCompatibleCharmap())
  //    return nullptr;

  return std::move(resultPattern);
}

RefPtr<FontFile> FontLoaderLinux::Load(const String16& family, int weight, bool italic) {
  FcUniquePtr<FcPattern> pattern = GetPatternForDescription(ToAscii(family), weight, italic, 12.0);

  if (pattern) {
    FcChar8* filepath = NULL;
	if (FcPatternGetString(pattern.get(), FC_FILE, 0, &filepath) == FcResultMatch) {
      return FontFile::Create(String16((const char*)filepath));
    }
  }

  return nullptr;
}

// Called from Platform.cpp
FontLoader* CreatePlatformFontLoader() {
    return new FontLoaderLinux();
}

}  // namespace ultralight
