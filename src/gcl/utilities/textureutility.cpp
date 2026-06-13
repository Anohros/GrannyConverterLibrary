#include "gcl/utilities/textureutility.h"

#include <vector>

#include <IL/ilu.h>

namespace GCL::Utilities {

GrannyTexture* getMaterialTexture(GrannyMaterial* grannyMaterial) {
    GrannyTexture* texture = nullptr;

    if (grannyMaterial->MapCount) {
        GrannyMaterialMap* map = &grannyMaterial->Maps[0];

        for (auto i = 0; i < grannyMaterial->MapCount; i++) {
            auto current_map = &grannyMaterial->Maps[i];
            auto is_color = _stricmp(current_map->Usage, "color") == 0 ||
                            _stricmp(current_map->Usage, "Diffuse color") == 0;
            auto is_texture = current_map->Material->Texture != nullptr;
            if (is_color && is_texture) {
                map = current_map;
                break;
            }
        }

        texture = map->Material->Texture;

        if (!texture && map->Material->MapCount) {
            texture = map->Material->Maps[0].Material->Texture;
        }
    } else {
        texture = grannyMaterial->Texture;
    }

    return texture;
}

void exportTexture(
    GrannyTexture* grannyTexture, const std::string& textureFilePath, bool flipImage
) {
    int bytesPerPixel;
    GrannyPixelLayout const* grannyPixelLayout;
    ILenum ilFormat;

    if (GrannyTextureHasAlpha(grannyTexture)) {
        bytesPerPixel = 4;
        grannyPixelLayout = GrannyRGBA8888PixelFormat;
        ilFormat = IL_RGBA;
    } else {
        bytesPerPixel = 3;
        grannyPixelLayout = GrannyRGB888PixelFormat;
        ilFormat = IL_RGB;
    }

    std::vector<uint8_t> pixels;
    pixels.resize(grannyTexture->Width * grannyTexture->Height * bytesPerPixel);

    GrannyCopyTextureImage(
        grannyTexture,
        0,
        0,
        grannyPixelLayout,
        grannyTexture->Width,
        grannyTexture->Height,
        grannyTexture->Width * bytesPerPixel,
        pixels.data()
    );

    ILuint imageId;
    ilGenImages(1, &imageId);
    ilBindImage(imageId);

    ilTexImage(
        grannyTexture->Width,
        grannyTexture->Height,
        1,
        bytesPerPixel,
        ilFormat,
        IL_UNSIGNED_BYTE,
        pixels.data()
    );

    if (flipImage) {
        iluFlipImage();
    }

    ilSave(IL_PNG, textureFilePath.c_str());
    ilDeleteImages(1, &imageId);
}

}  // namespace GCL::Utilities
