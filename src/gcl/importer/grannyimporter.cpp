#include "gcl/importer/grannyimporter.h"

#include <QDebug>
#include <filesystem>

namespace GCL::Importer {

GrannyImporter::GrannyImporter()
    : m_scene(new Scene())
{
    initialize();
}

GrannyImporter::GrannyImporter(Scene::SharedPtr scene)
    : m_scene(scene)
{
    initialize();
}

GrannyImporter::~GrannyImporter()
{
    for (const auto& grannyFile : m_importedGrannyFiles) {
        GrannyFreeFile(grannyFile);
    }

    delete m_importerMaterial;
    delete m_importerModel;
    delete m_importerSkeleton;
    delete m_importerAnimation;
}

void GrannyImporter::initialize()
{
    m_importerMaterial = new GrannyImporterMaterial(m_scene);
    m_importerModel = new GrannyImporterModel(m_scene);
    m_importerSkeleton = new GrannyImporterSkeleton(m_scene);
    m_importerAnimation = new GrannyImporterAnimation(m_scene);
}

void GrannyImporter::importFromFile(const char* grannyFilePath)
{
    qInfo("Import granny file (file: \"%s\") to scene.", grannyFilePath);

    GrannyFile* grannyFile = GrannyReadEntireFile(grannyFilePath);
    GrannyFileInfo* grannyFileInfo = GrannyGetFileInfo(grannyFile);

    m_importedGrannyFiles.push_back(grannyFile);

    // Add granny file base path as search path for textures.
    auto searchPath = filesystem::path(grannyFilePath).parent_path();
    if (!searchPath.empty()) {
        searchPath = searchPath.u8string().append("/");
        qDebug("Add search path \"%s\" to the scene.", searchPath.u8string().c_str());
        m_scene->addSearchPath(searchPath.u8string());
    }

    // Import all materials and models of the granny file to the scene.
    importMaterials(grannyFileInfo, grannyFilePath);
    importModels(grannyFileInfo, grannyFilePath);

    // Import all bones of the granny file to the scene.
    for (const auto& model : m_scene->getModels()) {
        model->setBones(m_importerSkeleton->loadBones(model->getData()));
    }

    // Import all animations of the granny file to the scene.
    importAnimations(grannyFileInfo, grannyFilePath);
}

void GrannyImporter::importMaterials(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load materials from granny file only if it has at least one material.
    if (!grannyFileInfo->MaterialCount) {
        qDebug("Skip load materials because granny file \"%s\" has no materials.", grannyFilePath);
        return;
    }

    qInfo("Import materials from granny file \"%s\".", grannyFilePath);
    m_importerMaterial->importMaterials(grannyFileInfo);
}

void GrannyImporter::importModels(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load models from granny file only if it has at least one model.
    if (!grannyFileInfo->ModelCount) {
        qDebug("Skip load models because granny file (file: \"%s\") has no models.", grannyFilePath);
        return;
    }

    qInfo("Import models from granny file \"%s\".", grannyFilePath);
    m_importerModel->importModels(grannyFileInfo);
}

void GrannyImporter::importAnimations(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load animations from granny file only if it has at least one animation.
    if (!grannyFileInfo->AnimationCount) {
        qDebug("Skip load animations because granny file (file: \"%s\") has no animations.", grannyFilePath);
        return;
    }

    qInfo("Import animations from granny file \"%s\".", grannyFilePath);
    m_importerAnimation->importAnimations(grannyFileInfo);
}

Scene::SharedPtr GrannyImporter::getScene() const
{
    return m_scene;
}

} // namespace GCL::Importer
