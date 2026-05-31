#pragma once

#include <iostream>

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
void convertImage(std::string sourceFilePath, std::string targetFilePath, bool flipImage = false);

}  // namespace GCL::Utilities
