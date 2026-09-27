#include "gcl/utilities/textureutility.h"

#include <vector>

#include <IL/ilu.h>

namespace GCL::Utilities {

GrannyTexture* getMaterialTexture(GrannyMaterial* grannyMaterial) {
    GrannyTexture* texture = nullptr;

    if (grannyMaterial->map_count) {
        GrannyMaterialMap* map = &grannyMaterial->maps[0];

        for (auto i = 0; i < grannyMaterial->map_count; i++) {
            auto current_map = &grannyMaterial->maps[i];
            auto is_color = _stricmp(current_map->usage, "color") == 0 ||
                            _stricmp(current_map->usage, "Diffuse color") == 0;
            auto is_texture = current_map->material->texture != nullptr;
            if (is_color && is_texture) {
                map = current_map;
                break;
            }
        }

        texture = map->material->texture;

        if (!texture && map->material->map_count) {
            texture = map->material->maps[0].material->texture;
        }
    } else {
        texture = grannyMaterial->texture;
    }

    return texture;
}

void exportTexture(
    GrannyTexture* grannyTexture, const std::string& textureFilePath, bool flipImage
) {
    ILubyte bytesPerPixel;
    GrannyPixelLayout const* grannyPixelLayout;
    ILenum ilFormat;

    auto& granny = Importer::GrannyLibrary::instance();

    if (granny.TextureHasAlpha(grannyTexture)) {
        bytesPerPixel = 4;
        grannyPixelLayout = granny.Rgba8888PixelFormat();
        ilFormat = IL_RGBA;
    } else {
        bytesPerPixel = 3;
        grannyPixelLayout = granny.Rgb888PixelFormat();
        ilFormat = IL_RGB;
    }

    std::vector<uint8_t> pixels;
    pixels.resize(grannyTexture->width * grannyTexture->height * bytesPerPixel);

    granny.CopyTextureImage(
        grannyTexture,
        0,
        0,
        grannyPixelLayout,
        grannyTexture->width,
        grannyTexture->height,
        grannyTexture->width * bytesPerPixel,
        pixels.data()
    );

    ILuint imageId;
    ilGenImages(1, &imageId);
    ilBindImage(imageId);

    ilTexImage(
        grannyTexture->width,
        grannyTexture->height,
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
