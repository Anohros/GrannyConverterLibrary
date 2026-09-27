#pragma once

#include <memory>

#include "gcl/exporter/fbxexporteranimation.h"
#include "gcl/exporter/fbxexportermaterial.h"
#include "gcl/exporter/fbxexportermesh.h"
#include "gcl/exporter/fbxexportermodulefactory.h"
#include "gcl/exporter/fbxexporterskeleton.h"
#include "gcl/exporter/fbxexportoptions.h"

namespace GCL::Exporter {

///
/// \brief Granny exporter - exports a scene to a filmbox file.
///
class FbxExporter {
public:
    ///
    /// \brief Constructer with data initialization.
    /// \param scene A scene to be exported.
    ///
    explicit FbxExporter(Bindings::Scene::SharedPtr scene);

    ///
    /// \brief Constructer with extended options and data initialization.
    /// \param options Export options to define what aspects of the scene needs to be exported.
    /// \param scene A scene to be exported.
    ///
    FbxExporter(FbxExportOptions options, Bindings::Scene::SharedPtr scene);

    ///
    /// \brief Constructer with extended options and data initialization.
    /// \param exporterModuleFactory Factory to create exporter modules.
    /// \param options Export options to define what aspects of the scene needs to be exported.
    /// \param scene A scene to be exported.
    ///
    FbxExporter(
        ExporterModuleFactoryInterface* exporterModuleFactory,
        FbxExportOptions options,
        Bindings::Scene::SharedPtr scene
    );

    ///
    /// \brief Destructor
    ///
    ~FbxExporter();

    ///
    /// \brief Export the scene to a filmbox file.
    /// \param outputFilepath Output path where the fbx file will be exported to.
    /// \return Returns whether the scene was exported successfully or not.
    ///
    bool exportToFile(const std::string& outputFilepath);

    ///
    /// \brief Export the models of the scene to the fbx scene.
    /// \param outputFilepath Output path where the fbx file will be exported to. It is required for
    /// material export.
    ///
    void exportModels(const std::string& outputFilepath);

    ///
    /// \brief Returns a scene.
    /// \return Scene
    ///
    Bindings::Scene::SharedPtr getScene() {
        return scene_;
    }

    ///
    /// \brief Returns fbx scene.
    /// \return Fbx scene
    ///
    FbxScene* getFbxScene() {
        return fbx_scene_;
    }

private:
    ///
    /// \brief Initializes the exporter. Called by the constructors, must not be called
    /// again as it would leak the previously created sdk objects and modules.
    ///
    void initialize();

protected:
    ///
    /// \brief Factory to create exporter modules.
    ///
    std::unique_ptr<ExporterModuleFactoryInterface> m_exporterModuleFactory;

    ///
    /// \brief Export options which define what aspects of the scene needs to be exported.
    ///
    FbxExportOptions m_options;

    ///
    /// \brief Material exporter module.
    ///
    std::unique_ptr<FbxExporterMaterial> m_exporterMaterial;

    ///
    /// \brief Mesh exporter module.
    ///
    std::unique_ptr<FbxExporterMesh> m_exporterMesh;

    ///
    /// \brief Skeleton exporter module.
    ///
    std::unique_ptr<FbxExporterSkeleton> m_exporterSkeleton;

    ///
    /// \brief Animation exporter module.
    ///
    std::unique_ptr<FbxExporterAnimation> m_exporterAnimation;

    ///
    /// \brief Scene of the importing granny file.
    ///
    Bindings::Scene::SharedPtr scene_;

    ///
    /// \brief Fbx manager for the export by the fbx sdk.
    ///
    FbxManager* m_fbxManager = nullptr;

    ///
    /// \brief Fbx scene for the export by the fbx sdk.
    ///
    FbxScene* fbx_scene_ = nullptr;
};

}  // namespace GCL::Exporter
