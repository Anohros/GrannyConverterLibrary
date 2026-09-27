#include "gcl/importer/grannyimportermodel.h"

#include "gcl/utilities/logging.h"

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

GrannyImporterModel::GrannyImporterModel(Bindings::Scene::SharedPtr scene) : scene_(scene) {
}

void GrannyImporterModel::importModels(GrannyFileInfo* grannyFileInfo) const {
    // Import each model of the granny model as scene model.
    for (unsigned i = 0; i < static_cast<unsigned>(grannyFileInfo->model_count); i++) {
        scene_->addModel(importModel(grannyFileInfo->models[i]));
    }
}

Bindings::Model::SharedPtr GrannyImporterModel::importModel(GrannyModel* grannyModel) const {
    info("Import granny model (name: \"%s\") as scene model.", grannyModel->name);

    const Bindings::Model::SharedPtr model = std::make_shared<Bindings::Model>(grannyModel);

    // Use granny method to translate initial placement in scene correctly.
    FbxMatrix transform;
    GrannyLibrary::instance().BuildCompositeTransform4x4(
        &grannyModel->initial_placement, reinterpret_cast<float*>(&transform)
    );

    model->setTransform(transform);
    model->setMeshes(importMeshes(grannyModel));

    return model;
}

std::vector<Bindings::Mesh::SharedPtr> GrannyImporterModel::importMeshes(
    GrannyModel* grannyModel
) const {
    unsigned meshBindingCount = static_cast<unsigned>(grannyModel->mesh_binding_count);

    // Create scene meshes of each mesh of the granny model.
    std::vector<Bindings::Mesh::SharedPtr> meshes;
    meshes.reserve(meshBindingCount);

    // Import each mesh of the granny model as scene mesh.
    for (unsigned i = 0; i < meshBindingCount; i++) {
        meshes.push_back(std::make_shared<Bindings::Mesh>(grannyModel->mesh_bindings[i].mesh));
    }

    return meshes;
}

Bindings::Mesh::SharedPtr GrannyImporterModel::importMesh(GrannyMesh* grannyMesh) const {
    return std::make_shared<Bindings::Mesh>(grannyMesh);
}

}  // namespace GCL::Importer
