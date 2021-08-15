#include "gcl/importer/grannyimportermaterial.h"

namespace GCL::Importer {

GrannyImporterMaterial::GrannyImporterMaterial(Scene::SharedPtr scene)
    : m_scene(scene)
{
}

GrannyImporterMaterial::~GrannyImporterMaterial()
{
}

void GrannyImporterMaterial::importMaterials(GrannyFileInfo* grannyFileInfo) const
{
    for (unsigned i = 0; i < static_cast<unsigned>(grannyFileInfo->MaterialCount); i++) {
        m_scene->addMaterial(make_shared<Material>(grannyFileInfo->Materials[i]));
    }
}

} // namespace GCL::Importer
