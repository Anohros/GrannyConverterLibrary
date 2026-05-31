#include "gcl/exporter/fbxexporter.h"

#include "gcl/utilities/fbxsdkcommon.h"

namespace GCL::Exporter {

using namespace GCL::Utilities;

FbxExporter::FbxExporter(Scene::SharedPtr scene) : scene_(scene) {
    initialize();
}

FbxExporter::FbxExporter(FbxExportOptions options, Scene::SharedPtr scene)
    : m_options(options), scene_(scene) {
    initialize();
}

FbxExporter::FbxExporter(
    ExporterModuleFactoryInterface* exportModuleFactory,
    FbxExportOptions options,
    Scene::SharedPtr scene
)
    : m_options(options), scene_(scene) {
    m_exporterModuleFactory = exportModuleFactory;
    initialize();
}

FbxExporter::~FbxExporter() {
    FbxSdkCommon::DestroySdkObjects(m_fbxManager);

    if (m_exporterModuleFactory) {
        delete m_exporterModuleFactory;
    }

    delete m_exporterMaterial;
    delete m_exporterSkeleton;
    delete m_exporterAnimation;
}

void FbxExporter::initialize() {
    // Initialize the fbx sdk.
    FbxSdkCommon::InitializeSdkObjects(m_fbxManager, fbx_scene_);

    // Convert the axis coordinate system.
    FbxAxisSystem axisSystem;
    FbxAxisSystem::ParseAxisSystem(m_options.convertAxis.c_str(), axisSystem);
    axisSystem.ConvertScene(fbx_scene_);

    if (!m_exporterModuleFactory) {
        m_exporterModuleFactory = new FbxExporterModuleFactory();
    }

    m_exporterMaterial = m_exporterModuleFactory->createExporterModuleMaterial(scene_, fbx_scene_);
    m_exporterMesh = m_exporterModuleFactory->createExporterModuleMesh(scene_, fbx_scene_);
    m_exporterSkeleton = m_exporterModuleFactory->createExporterModuleSkeleton(scene_, fbx_scene_);
    m_exporterAnimation =
        m_exporterModuleFactory->createExporterModuleAnimation(scene_, fbx_scene_);
}

void FbxExporter::exportToFile(string outputFilepath) {
    exportModels(outputFilepath);

    FbxSdkCommon::SaveScene(m_fbxManager, fbx_scene_, outputFilepath.c_str(), false, false);
}

void FbxExporter::exportModels(string outputFilepath) {
    if (m_options.exportMaterials) {
        m_exporterMaterial->exportMaterials(outputFilepath);
    }

    for (auto model : scene_->getModels()) {
        // Export skeleton if enabled.
        if (m_options.exportSkeleton && model->getBones().size() > 0) {
            auto exist = fbx_scene_->GetRootNode()->FindChild(model->getBones()[0]->getData().Name);

            if (!exist) {
                m_exporterSkeleton->exportBones(model);
            } else {
                for (auto otherModel : scene_->getModels()) {
                    if (otherModel != model && otherModel->getBones()[0]->getNode() == exist) {
                        model->setBones(otherModel->getBones());
                    }
                }
            }
        }

        if (model->isExcluded()) {
            continue;
        }

        if (m_options.exportMeshes) {
            m_exporterMesh->exportMeshes(model, m_options.exportSkeleton);
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
