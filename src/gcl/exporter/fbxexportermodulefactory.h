#pragma once

#include "fbxsdk.h"
#include "gcl/bindings/scene.h"
#include "gcl/exporter/fbxexporteranimation.h"
#include "gcl/exporter/fbxexportermaterial.h"
#include "gcl/exporter/fbxexportermesh.h"
#include "gcl/exporter/fbxexporterskeleton.h"

namespace GCL::Exporter {

///
/// \brief The ExporterModuleFactoryInterface interface.
///
class ExporterModuleFactoryInterface {
public:
    ///
    /// \brief Destructor
    ///
    virtual ~ExporterModuleFactoryInterface() = default;

    ///
    /// \brief Returns material exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for materials.
    ///
    virtual FbxExporterMaterial* createExporterModuleMaterial(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) = 0;

    ///
    /// \brief Returns mesh exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for meshes.
    ///
    virtual FbxExporterMesh* createExporterModuleMesh(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) = 0;

    ///
    /// \brief Returns skeleton exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for skeletons.
    ///
    virtual FbxExporterSkeleton* createExporterModuleSkeleton(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) = 0;

    ///
    /// \brief Returns animation exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for animations.
    ///
    virtual FbxExporterAnimation* createExporterModuleAnimation(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) = 0;
};

///
/// \brief The ExporterModuleFactory class.
///
class FbxExporterModuleFactory : public ExporterModuleFactoryInterface {
public:
    ///
    /// \brief Returns material exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for materials.
    ///
    FbxExporterMaterial* createExporterModuleMaterial(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) override;

    ///
    /// \brief Returns mesh exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for meshes.
    ///
    FbxExporterMesh* createExporterModuleMesh(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) override;

    ///
    /// \brief Returns skeleton exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for skeletons.
    ///
    FbxExporterSkeleton* createExporterModuleSkeleton(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) override;

    ///
    /// \brief Returns animation exporter module.
    /// \param scene Scene which needs to be exported.
    /// \param fbxScene Fbx scene which has to be used for the export.
    /// \return Exporter module for animations.
    ///
    FbxExporterAnimation* createExporterModuleAnimation(
        Bindings::Scene::SharedPtr scene, FbxScene* fbxScene
    ) override;
};

}  // namespace GCL::Exporter
