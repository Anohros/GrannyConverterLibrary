#pragma once

#include <string>

namespace GCL::Utilities {

///
/// \brief Initializes devil image library.
///
void initializeDevilImageLibrary();

///
/// \brief Shutdown devil image library.
///
void shutdownDevilImageLibrary();

///
/// \brief Converts a image using devil image library.
/// \param sourceFilePath
/// \param targetFilePath
/// \param flipImage
///
void convertImage(
    const std::string& sourceFilePath, const std::string& targetFilePath, bool flipImage = false
);

}  // namespace GCL::Utilities
