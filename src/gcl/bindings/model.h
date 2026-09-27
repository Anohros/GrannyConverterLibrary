#pragma once

#include <vector>

#include <fbxsdk.h>

#include "gcl/bindings/binding.h"
#include "gcl/bindings/bone.h"
#include "gcl/bindings/mesh.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Bindings {

///
/// \brief Binding of granny model data and the counterparts data like meshes and bones.
///
class Model : public Binding<Model> {
public:
    ///
    /// \brief Constructer
    /// \param data Granny data of the model.
    ///
    explicit Model(GrannyModel* data);

    ///
    /// \brief Returns the granny model data.
    /// \return Granny model data
    ///
    GrannyModel* getData();

    ///
    /// \brief Returns all meshes of the model.
    /// \return Meshes
    ///
    std::vector<Mesh::SharedPtr> getMeshes();

    ///
    /// \brief Returns all bones of the model.
    /// \return Model bones
    ///
    std::vector<Bone::SharedPtr> getBones();

    ///
    /// \brief Append meshes to the scene.
    /// \param Meshes
    ///
    void setMeshes(std::vector<Mesh::SharedPtr> meshes);

    ///
    /// \brief Append bones to the scene.
    /// \param Model bones
    ///
    void setBones(std::vector<Bone::SharedPtr> bones);

    ///
    /// \brief Returns if model has rigid body meshes.
    /// \return Returns whether model has at least one rigid meshes or not.
    ///
    [[nodiscard]] bool hasRigidMeshes() const;

    ///
    /// \brief Set transform of the model.
    /// \param Model transform
    ///
    void setTransform(const FbxMatrix& transform);

protected:
    ///
    /// \brief Granny data of the model.
    ///
    GrannyModel* m_data = nullptr;

    ///
    /// \brief Meshes of the model.
    ///
    std::vector<Mesh::SharedPtr> m_meshes;

    ///
    /// \brief Bones of the model.
    ///
    std::vector<Bone::SharedPtr> m_bones;

    ///
    /// \brief Transform of the model.
    ///
    FbxMatrix m_transform;
};

}  // namespace GCL::Bindings
