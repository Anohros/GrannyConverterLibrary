#pragma once

#include "gcl/bindings/bone.h"
#include "gcl/bindings/mesh.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/grannyformat.h"

#include <vector>

namespace GCL::Importer {

using namespace std;
using namespace GCL::Bindings;

///
/// \brief The GrannyImporterModel class.
///
class GrannyImporterModel {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    GrannyImporterModel(Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    ~GrannyImporterModel();

    ///
    /// \brief Imports a model and all its meshes from a granny file to the scene.
    /// \param grannyFileInfo Granny file info
    ///
    void importModels(GrannyFileInfo* grannyFileInfo) const;

protected:
    ///
    /// \brief Imports a granny model as scene model.
    /// \param grannyModel Granny model.
    /// \return Model of the granny model.
    ///
    Model::SharedPtr importModel(GrannyModel* grannyModel) const;

    ///
    /// \brief Import all meshes from a granny model as scene meshes.
    /// \param grannyModel Granny model
    /// \return All meshes of the granny model.
    ///
    vector<Mesh::SharedPtr> importMeshes(GrannyModel* grannyModel) const;

    ///
    /// \brief Imports a granny mesh as scene mesh.
    /// \param grannyMesh Granny mesh
    /// \return grannyMesh Mesh of the granny mesh.
    ///
    Mesh::SharedPtr importMesh(GrannyMesh* grannyMesh) const;

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Scene::SharedPtr m_scene;
};

} // namespace GCL::Importer
