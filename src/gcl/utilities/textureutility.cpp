#include "gcl/utilities/textureutility.h"

namespace GCL::Utilities {

GrannyTexture* getMaterialTexture(GrannyMaterial* grannyMaterial)
{
    GrannyTexture* texture = nullptr;

    if (grannyMaterial->MapCount) {
        texture = grannyMaterial->Maps[0].Material->Texture;

        if (!texture && grannyMaterial->Maps[0].Material->MapCount) {
            texture = grannyMaterial->Maps[0].Material->Maps[0].Material->Texture;
        }
    } else {
        texture = grannyMaterial->Texture;
    }

    return texture;
}

} // namespace GCL::Utilities
