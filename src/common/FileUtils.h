#pragma once
#include <AppCore/Defines.h>

namespace ultralight {
namespace FileUtils {

/**
* @brief Get the mime type of a file based on its extension.
* 
* @param ext The file extension.
* @return const char* The mime type of the file.
*/
const char* FileExtensionToMimeType(const char* ext);

} // namespace FileUtils
} // namespace ultralight