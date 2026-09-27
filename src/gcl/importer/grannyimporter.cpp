#include "gcl/importer/grannyimporter.h"

#include <filesystem>

#include "gcl/utilities/logging.h"

namespace GCL::Importer {

using namespace GCL::Utilities::Logging;

GrannyImporter::GrannyImporter() : scene_(new Bindings::Scene()) {
    initialize();
}

GrannyImporter::GrannyImporter(Bindings::Scene::SharedPtr scene) : scene_(scene) {
    initialize();
}

GrannyImporter::GrannyImporter(GrannyImportOptions options)
    : m_options(options), scene_(new Bindings::Scene()) {
    initialize();
}

GrannyImporter::GrannyImporter(GrannyImportOptions options, Bindings::Scene::SharedPtr scene)
    : m_options(options), scene_(scene) {
    initialize();
}

GrannyImporter::~GrannyImporter() {
    // The importer modules are released by their unique_ptr members.
    auto& granny = GrannyLibrary::instance();
    for (const auto& grannyFile : m_importedGrannyFiles) {
        granny.FreeFile(grannyFile);
    }
}

void GrannyImporter::initialize() {
    m_importerMaterial = std::make_unique<GrannyImporterMaterial>(scene_);
    m_importerModel = std::make_unique<GrannyImporterModel>(scene_);
    m_importerSkeleton = std::make_unique<GrannyImporterSkeleton>(scene_);

    // If option for deboor animation importer is enabled then use
    // deboor animation importer otherwise default animation importer.
    if (m_options.importAnimationDeboor) {
        m_importerAnimation = std::make_unique<GrannyImporterAnimationDeboor>(scene_);
    } else {
        m_importerAnimation = std::make_unique<GrannyImporterAnimation>(scene_);
    }
}

bool GrannyImporter::importFromFile(const std::string& grannyFilePath) {
    if (!std::filesystem::exists(grannyFilePath)) {
        warning("Skip import from file. File \"%s\" was not found.", grannyFilePath.c_str());
        return false;
    }

    info("Import granny file (file: \"%s\") to scene.", grannyFilePath.c_str());

    auto& granny = GrannyLibrary::instance();
    GrannyFile* grannyFile = granny.ReadEntireFile(grannyFilePath);
    if (!grannyFile) {
        warning("Skip import from file. File \"%s\" could not be read.", grannyFilePath.c_str());
        return false;
    }

    GrannyFileInfo* grannyFileInfo = granny.GetFileInfo(grannyFile);

    // Add granny file to list of imported granny files.
    m_importedGrannyFiles.push_back(grannyFile);

    // Add granny file path to the imported file paths of the scene.
    scene_->addImportedFilePath(grannyFilePath);

    // Add granny file base path as search path for textures.
    auto searchPath = std::filesystem::path(grannyFilePath).parent_path();
    if (!searchPath.empty()) {
        searchPath = searchPath.append("/");
        debug("Add search path \"%s\" to the scene.", searchPath.c_str());
        scene_->addSearchPath(searchPath.string());
    }

    // Import all materials and models of the granny file to the scene.
    importMaterials(grannyFileInfo, grannyFilePath);
    importModels(grannyFileInfo, grannyFilePath);

    // Import all bones of the granny file to the scene.
    for (const auto& model : scene_->getModels()) {
        model->setBones(m_importerSkeleton->loadBones(model->getData()));
    }

    // Import all animations of the granny file to the scene.
    importAnimations(grannyFileInfo, grannyFilePath);

    return true;
}

void GrannyImporter::importMaterials(
    GrannyFileInfo* grannyFileInfo, const std::string& grannyFilePath
) {
    // Load materials from granny file only if it has at least one material.
    if (!grannyFileInfo->material_count) {
        debug(
            "Skip load materials because granny file \"%s\" has no materials.",
            grannyFilePath.c_str()
        );
        return;
    }

    info("Import materials from granny file \"%s\".", grannyFilePath.c_str());
    m_importerMaterial->importMaterials(grannyFileInfo);
}

void GrannyImporter::importModels(
    GrannyFileInfo* grannyFileInfo, const std::string& grannyFilePath
) {
    // Load models from granny file only if it has at least one model.
    if (!grannyFileInfo->model_count) {
        debug(
            "Skip load models because granny file (file: \"%s\") has no models.",
            grannyFilePath.c_str()
        );
        return;
    }

    info("Import models from granny file \"%s\".", grannyFilePath.c_str());
    m_importerModel->importModels(grannyFileInfo);
}

void GrannyImporter::importAnimations(
    GrannyFileInfo* grannyFileInfo, const std::string& grannyFilePath
) {
    // Load animations from granny file only if it has at least one animation.
    if (!grannyFileInfo->animation_count) {
        debug(
            "Skip load animations because granny file (file: \"%s\") has no animations.",
            grannyFilePath.c_str()
        );
        return;
    }

    info("Import animations from granny file \"%s\".", grannyFilePath.c_str());
    m_importerAnimation->ImportAnimations(grannyFileInfo);
}

Bindings::Scene::SharedPtr GrannyImporter::getScene() const {
    return scene_;
}

}  // namespace GCL::Importer
