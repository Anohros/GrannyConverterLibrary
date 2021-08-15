#pragma once

#include "gcl/bindings/bone.h"
#include "gcl/bindings/scene.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Importer {

using namespace std;
using namespace GCL::Bindings;

///
/// \brief The GrannyImporterMaterial class.
///
class GrannyImporterMaterial {
public:
    ///
    /// \brief Constructor
    /// \param scene Scene which needs to be exported.
    ///
    GrannyImporterMaterial(Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    ~GrannyImporterMaterial();

    ///
    /// \brief Import all materials of a granny file as scene materials to the scene.
    /// \param GrannyFileInfo Granny file info
    ///
    void importMaterials(GrannyFileInfo* grannyFileInfo) const;

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Scene::SharedPtr m_scene;
};

} // namespace GCL::Importer
