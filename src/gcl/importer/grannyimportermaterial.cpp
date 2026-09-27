#include "gcl/importer/grannyimportermaterial.h"

namespace GCL::Importer {

GrannyImporterMaterial::GrannyImporterMaterial(Bindings::Scene::SharedPtr scene) : scene_(scene) {
}

void GrannyImporterMaterial::importMaterials(GrannyFileInfo* grannyFileInfo) const {
    for (unsigned i = 0; i < static_cast<unsigned>(grannyFileInfo->material_count); i++) {
        scene_->addMaterial(std::make_shared<Bindings::Material>(grannyFileInfo->materials[i]));
    }
}

}  // namespace GCL::Importer
