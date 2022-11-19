#pragma once

#include "gcl/importer/grannyformat.h"

namespace GCL::Utilities {

///
/// \brief Returns diffuse texture of a granny material.
/// \param grannyMaterial Granny material
/// \return Diffuse texture
///
GrannyTexture* getMaterialTexture(GrannyMaterial* grannyMaterial);

///
/// \brief Exports a embedded texture from granny2 to a file.
/// \param grannyTexture Granny texture
/// \param textureFilePath Export file path for the granny texture.
/// \param flipImage If image should be flipped.
///
void exportTexture(GrannyTexture* grannyTexture, string textureFilePath, bool flipImage = false);

} // namespace GCL::Utilities
