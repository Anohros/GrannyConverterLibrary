#include "gcl/importer/grannyimporter.h"

#include "gcl/utilities/logging.h"

#include <filesystem>

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

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

GrannyImporter::GrannyImporter(GrannyImportOptions options)
    : m_options(options)
    , m_scene(new Scene())
{
    initialize();
}

GrannyImporter::GrannyImporter(GrannyImportOptions options, Scene::SharedPtr scene)
    : m_options(options)
    , m_scene(scene)
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

    // If option for deboor animation importer is enabled then use
    // deboor animation importer otherwise default animation importer.
    if (m_options.importAnimationDeboor) {
        m_importerAnimation = new GrannyImporterAnimationDeboor(m_scene);
    } else {
        m_importerAnimation = new GrannyImporterAnimation(m_scene);
    }
}

bool GrannyImporter::importFromFile(const char* grannyFilePath)
{
    if (!ifstream(grannyFilePath).is_open()) {
        warning("Skip import from file. File \"%s\" was not found.", grannyFilePath);
        return false;
    }

    info("Import granny file (file: \"%s\") to scene.", grannyFilePath);

    GrannyFile* grannyFile = GrannyReadEntireFile(grannyFilePath);
    GrannyFileInfo* grannyFileInfo = GrannyGetFileInfo(grannyFile);

    // Add granny file to list of imported granny files.
    m_importedGrannyFiles.push_back(grannyFile);

    // Add granny file path to the imported file paths of the scene.
    m_scene->addImportedFilePath(grannyFilePath);

    // Add granny file base path as search path for textures.
    auto searchPath = filesystem::path(grannyFilePath).parent_path();
    if (!searchPath.empty()) {
        searchPath = searchPath.u8string().append("/");
        debug("Add search path \"%s\" to the scene.", searchPath.u8string().c_str());
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

    return true;
}

void GrannyImporter::importMaterials(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load materials from granny file only if it has at least one material.
    if (!grannyFileInfo->MaterialCount) {
        debug("Skip load materials because granny file \"%s\" has no materials.", grannyFilePath);
        return;
    }

    info("Import materials from granny file \"%s\".", grannyFilePath);
    m_importerMaterial->importMaterials(grannyFileInfo);
}

void GrannyImporter::importModels(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load models from granny file only if it has at least one model.
    if (!grannyFileInfo->ModelCount) {
        debug("Skip load models because granny file (file: \"%s\") has no models.", grannyFilePath);
        return;
    }

    info("Import models from granny file \"%s\".", grannyFilePath);
    m_importerModel->importModels(grannyFileInfo);
}

void GrannyImporter::importAnimations(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath)
{
    // Load animations from granny file only if it has at least one animation.
    if (!grannyFileInfo->AnimationCount) {
        debug("Skip load animations because granny file (file: \"%s\") has no animations.", grannyFilePath);
        return;
    }

    info("Import animations from granny file \"%s\".", grannyFilePath);
    m_importerAnimation->importAnimations(grannyFileInfo);
}

Scene::SharedPtr GrannyImporter::getScene() const
{
    return m_scene;
}

} // namespace GCL::Importer
