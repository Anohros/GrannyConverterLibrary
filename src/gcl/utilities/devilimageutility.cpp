#include "gcl/utilities/devilimageutility.h"

#include <IL/ilu.h>

namespace GCL::Utilities {

void initializeDevilImageLibrary()
{
    ilInit();
}

void shutdownDevilImageLibrary()
{
    ilShutDown();
}

void convertImage(string sourceFilePath, string targetFilePath, bool flipImage)
{
    ILuint imageId;
    ilGenImages(1, &imageId);
    ilBindImage(imageId);
    ilLoadImage(sourceFilePath.c_str());

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    if (flipImage) {
        iluFlipImage();
    }

    ilSaveImage(targetFilePath.c_str());
    ilDeleteImage(imageId);
}

} // namespace GCL::Utilities
