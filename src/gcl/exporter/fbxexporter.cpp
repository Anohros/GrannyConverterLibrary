#include "gcl/exporter/fbxexporter.h"

#include <map>
#include <stdexcept>
#include <string>

#include "gcl/utilities/fbxsdkcommon.h"
#include "gcl/utilities/logging.h"

namespace GCL::Exporter {

using namespace GCL::Utilities::Logging;

FbxExporter::FbxExporter(Bindings::Scene::SharedPtr scene) : scene_(std::move(scene)) {
    initialize();
}

FbxExporter::FbxExporter(FbxExportOptions options, Bindings::Scene::SharedPtr scene)
    : m_options(std::move(options)), scene_(std::move(scene)) {
    initialize();
}

FbxExporter::FbxExporter(
    ExporterModuleFactoryInterface* exporterModuleFactory,
    FbxExportOptions options,
    Bindings::Scene::SharedPtr scene
)
    : m_options(std::move(options)), scene_(std::move(scene)) {
    m_exporterModuleFactory =
        std::unique_ptr<ExporterModuleFactoryInterface>(exporterModuleFactory);
    initialize();
}

FbxExporter::~FbxExporter() {
    Utilities::FbxSdkCommon::DestroySdkObjects(m_fbxManager);
}

void FbxExporter::initialize() {
    // Validate the axis coordinate system conversion string before initializing.
    FbxAxisSystem axisSystem;
    if (!FbxAxisSystem::ParseAxisSystem(m_options.convertAxis.c_str(), axisSystem)) {
        fatal(
            "Invalid axis coordinate system \"%s\" for the scene conversion.",
            m_options.convertAxis.c_str()
        );
        throw std::invalid_argument("Invalid axis coordinate system: " + m_options.convertAxis);
    }

    // Initialize the fbx sdk.
    Utilities::FbxSdkCommon::InitializeSdkObjects(m_fbxManager, fbx_scene_);

    // Convert the axis coordinate system.
    axisSystem.ConvertScene(fbx_scene_);

    if (!m_exporterModuleFactory) {
        m_exporterModuleFactory = std::make_unique<FbxExporterModuleFactory>();
    }

    m_exporterMaterial = std::unique_ptr<FbxExporterMaterial>(
        m_exporterModuleFactory->createExporterModuleMaterial(scene_, fbx_scene_)
    );
    m_exporterMesh = std::unique_ptr<FbxExporterMesh>(
        m_exporterModuleFactory->createExporterModuleMesh(scene_, fbx_scene_)
    );
    m_exporterSkeleton = std::unique_ptr<FbxExporterSkeleton>(
        m_exporterModuleFactory->createExporterModuleSkeleton(scene_, fbx_scene_)
    );
    m_exporterAnimation = std::unique_ptr<FbxExporterAnimation>(
        m_exporterModuleFactory->createExporterModuleAnimation(scene_, fbx_scene_)
    );
}

bool FbxExporter::exportToFile(const std::string& outputFilepath) {
    exportModels(outputFilepath);

    return Utilities::FbxSdkCommon::SaveScene(
        m_fbxManager, fbx_scene_, outputFilepath.c_str(), false, false
    );
}

void FbxExporter::exportModels(const std::string& outputFilepath) {
    if (m_options.exportMaterials) {
        m_exporterMaterial->exportMaterials(outputFilepath);
    }

    // Pre-pass: group the models by their first bone name so that skeletons are only
    // exported once per skeleton. Models sharing a skeleton reuse the bones of the
    // model which owns the skeleton.
    std::map<std::string, Bindings::Model::SharedPtr> skeletonOwners;
    if (m_options.exportSkeleton) {
        for (const auto& model : scene_->getModels()) {
            const auto& bones = model->getBones();
            if (bones.empty()) {
                continue;
            }

            const std::string firstBoneName = bones[0]->getData().Name;
            auto existingOwner = skeletonOwners.find(firstBoneName);
            if (existingOwner != skeletonOwners.end()) {
                model->setBones(existingOwner->second->getBones());
            } else {
                skeletonOwners.emplace(firstBoneName, model);
            }
        }
    }

    for (const auto& model : scene_->getModels()) {
        // Export the skeleton if enabled and this model owns the skeleton.
        const auto& bones = model->getBones();
        if (m_options.exportSkeleton && !bones.empty() &&
            skeletonOwners.at(bones[0]->getData().Name) == model) {
            m_exporterSkeleton->exportBones(model);
        }

        if (model->isExcluded()) {
            continue;
        }

        if (m_options.exportMeshes) {
            m_exporterMesh->ExportMeshes(model, m_options.exportSkeleton);
        }

        if (m_options.exportSkeleton && model->getBones().size() > 1) {
            m_exporterSkeleton->exportPoses(model);
        }
    }

    if (m_options.exportAnimation) {
        m_exporterAnimation->exportAnimations();
    }
}

}  // namespace GCL::Exporter
