#include "gcl/utilities/devilimageutility.h"

#include <IL/il.h>
#include <IL/ilu.h>

namespace GCL::Utilities {

void initializeDevilImageLibrary() {
    ilInit();
}

void shutdownDevilImageLibrary() {
    ilShutDown();
}

void convertImage(
    const std::string& sourceFilePath, const std::string& targetFilePath, bool flipImage
) {
    // Validate input parameters.
    if (sourceFilePath.empty() || targetFilePath.empty()) {
        return;
    }

    // Initialize DevIL image
    ILuint imageId;
    ilGenImages(1, &imageId);
    ilBindImage(imageId);

    // Load the source image. Cleanup resources on failure.
    if (ilLoadImage(sourceFilePath.c_str()) == 0U) {
        ilDeleteImages(1, &imageId);
        return;
    }

    // Convert to RGBA format.
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    // Flip image if requested.
    if (flipImage) {
        iluFlipImage();
    }

    // Save the converted image to the target path.
    ilSaveImage(targetFilePath.c_str());

    // Cleanup resources.
    ilDeleteImages(1, &imageId);
}

}  // namespace GCL::Utilities
