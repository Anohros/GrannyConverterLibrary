#include "gcl/importer/grannyimportermaterial.h"

namespace GCL::Importer {

GrannyImporterMaterial::GrannyImporterMaterial(Bindings::Scene::SharedPtr scene) : m_scene(scene) {
}

void GrannyImporterMaterial::importMaterials(GrannyFileInfo* grannyFileInfo) const {
    for (unsigned i = 0; i < static_cast<unsigned>(grannyFileInfo->MaterialCount); i++) {
        m_scene->addMaterial(std::make_shared<Bindings::Material>(grannyFileInfo->Materials[i]));
    }
}

}  // namespace GCL::Importer
