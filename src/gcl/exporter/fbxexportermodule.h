#pragma once

#include <fbxsdk.h>

#include "gcl/bindings/scene.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Exporter {

///
/// \brief The ExporterModule class.
///
class FbxExporterModule {
public:
    ///
    /// \brief ExporterModule
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    ///
    FbxExporterModule(Bindings::Scene::SharedPtr scene, FbxScene* fbxScene);

    ///
    /// \brief Destructor
    ///
    ~FbxExporterModule();

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Bindings::Scene::SharedPtr scene_;

    ///
    /// \brief Fbx scene for the export.
    ///
    FbxScene* fbx_scene_ = nullptr;
};

}  // namespace GCL::Exporter
