#include "gcl/importer/grannyimportermodel.h"

#include "gcl/utilities/logging.h"

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

GrannyImporterModel::GrannyImporterModel(Scene::SharedPtr scene)
    : m_scene(scene)
{
}

GrannyImporterModel::~GrannyImporterModel()
{
}

void GrannyImporterModel::importModels(GrannyFileInfo* grannyFileInfo) const
{
    // Import each model of the granny model as scene model.
    for (unsigned i = 0; i < static_cast<unsigned>(grannyFileInfo->ModelCount); i++) {
        m_scene->addModel(importModel(grannyFileInfo->Models[i]));
    }
}

Model::SharedPtr GrannyImporterModel::importModel(GrannyModel* grannyModel) const
{
    info("Import granny model (name: \"%s\") as scene model.", grannyModel->Name);

    const Model::SharedPtr model = make_shared<Model>(grannyModel);

    // Use granny method to translate initial placement in scene correctly.
    FbxMatrix transform;
    GrannyBuildCompositeTransform4x4(&grannyModel->InitialPlacement, reinterpret_cast<float*>(&transform));

    model->setTransform(transform);
    model->setMeshes(importMeshes(grannyModel));

    return model;
}

vector<Mesh::SharedPtr> GrannyImporterModel::importMeshes(GrannyModel* grannyModel) const
{
    unsigned meshBindingCount = static_cast<unsigned>(grannyModel->MeshBindingCount);

    // Create scene meshes of each mesh of the granny model.
    vector<Mesh::SharedPtr> meshes;
    meshes.reserve(meshBindingCount);

    // Import each mesh of the granny model as scene mesh.
    for (unsigned i = 0; i < meshBindingCount; i++) {
        meshes.push_back(make_shared<Mesh>(grannyModel->MeshBindings[i].Mesh));
    }

    return meshes;
}

Mesh::SharedPtr GrannyImporterModel::importMesh(GrannyMesh* grannyMesh) const
{
    return make_shared<Mesh>(grannyMesh);
}

} // namespace GCL::Importer
