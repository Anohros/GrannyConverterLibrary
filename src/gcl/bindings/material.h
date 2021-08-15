#pragma once

#include "gcl/bindings/binding.h"
#include "gcl/importer/grannyformat.h"

#include <fbxsdk.h>

#include <vector>

namespace GCL::Bindings {

///
/// \brief Binding of granny material data.
///
class Material : public Binding<Material> {
public:
    ///
    /// \brief Constructer
    /// \param data Granny data of the material.
    ///
    Material(GrannyMaterial* data);

    ///
    /// \brief Returns the granny material data.
    /// \return Granny material data
    ///
    GrannyMaterial* getData();

    ///
    /// \brief Returns the fbx node of the material.
    /// \return Fbx mesh node
    ///
    FbxSurfaceMaterial* getNode();

    ///
    /// \brief Sets the granny material data.
    /// \param data Granny material data
    ///
    void setData(GrannyMaterial* data);

    ///
    /// \brief Sets the fbx node of the material.
    /// \param Fbx mesh node
    ///
    void setNode(FbxSurfaceMaterial* node);

protected:
    ///
    /// \brief Granny data of the material.
    ///
    GrannyMaterial* m_data = nullptr;

    ///
    /// \brief Fbx node of the material.
    ///
    FbxSurfaceMaterial* m_node = nullptr;
};

} // namespace GCL::Bindings
