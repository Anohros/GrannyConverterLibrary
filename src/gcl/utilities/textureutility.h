#pragma once

#include "gcl/importer/grannyformat.h"

namespace GCL::Utilities {

///
/// \brief Returns diffuse texture of a granny material.
/// \param grannyMaterial Granny material
/// \return Diffuse texture
///
GrannyTexture* getMaterialTexture(GrannyMaterial* grannyMaterial);

} // namespace GCL::Utilities
