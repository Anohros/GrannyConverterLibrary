#pragma once

#include "gcl/bindings/animation.h"
#include "gcl/bindings/bone.h"
#include "gcl/bindings/curvepositionkey.h"
#include "gcl/bindings/curverotationkey.h"
#include "gcl/bindings/curvescalekey.h"
#include "gcl/bindings/material.h"
#include "gcl/bindings/mesh.h"
#include "gcl/bindings/model.h"
#include "gcl/bindings/scene.h"
#include "gcl/bindings/track.h"
#include "gcl/importer/deboor.h"
#include "gcl/importer/grannyformat.h"
#include "gcl/importer/grannyimporteranimation.h"
#include "gcl/importer/grannyimporteranimation_deboor.h"
#include "gcl/importer/grannyimportermaterial.h"
#include "gcl/importer/grannyimportermodel.h"
#include "gcl/importer/grannyimporterskeleton.h"

#include <vector>

namespace GCL::Importer {

using namespace std;
using namespace GCL::Bindings;

///
/// \brief Granny importer - imports a scene from a granny file.
///
class GrannyImporter {
public:
    ///
    /// \brief Empty scene constructor
    ///
    GrannyImporter();

    ///
    /// \brief Constructor with data initialization to reuse a already initialized scene.
    /// \param scene Already initialized scene to be reused.
    ///
    GrannyImporter(Scene::SharedPtr scene);

    ///
    /// \brief Destructor
    ///
    ~GrannyImporter();

    ///
    /// \brief Initializes the exporter.
    ///
    void initialize();

    ///
    /// \brief Import the granny scene.
    /// \param Full filepath to the importing granny file.
    /// \return Returns status whether file was imported successfully or skipped.
    ///
    bool importFromFile(const char* fullFilePath);

    ///
    /// \brief Load and add materials from granny file to the scene.
    /// \param grannyFileInfo Granny file info
    /// \param grannyFilePath Granny file path
    ///
    void importMaterials(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath);

    ///
    /// \brief Load and add models from granny file to the scene.
    /// \param grannyFileInfo Granny file info
    /// \param grannyFilePath Granny file path
    ///
    void importModels(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath);

    ///
    /// \brief Load and add animations from granny file to the scene.
    /// \param grannyFileInfo Granny file info
    /// \param grannyFilePath Granny file path
    ///
    void importAnimations(GrannyFileInfo* grannyFileInfo, const char* grannyFilePath);

    ///
    /// \brief Return the imported scene.
    /// \return Imported scene
    ///
    Scene::SharedPtr getScene() const;

protected:
    ///
    /// \brief Scene of the importing granny file.
    ///
    Scene::SharedPtr m_scene;

    ///
    /// \brief Material importer module.
    ///
    GrannyImporterMaterial* m_importerMaterial = nullptr;

    ///
    /// \brief Model importer module.
    ///
    GrannyImporterModel* m_importerModel = nullptr;

    ///
    /// \brief Skeleton importer module.
    ///
    GrannyImporterSkeleton* m_importerSkeleton = nullptr;

    ///
    /// \brief Animation importer module.
    ///
    GrannyImporterAnimation* m_importerAnimation = nullptr;

    ///
    /// \brief Granny files of the imported granny files.
    ///
    vector<GrannyFile*> m_importedGrannyFiles;
};

} // namespace GCL::Importer
