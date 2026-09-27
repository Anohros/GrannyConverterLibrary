#pragma once

#include <algorithm>
#include <set>
#include <string>
#include <vector>

#include "gcl/bindings/animation.h"
#include "gcl/bindings/material.h"
#include "gcl/bindings/model.h"
#include "gcl/importer/grannyformat.h"

namespace GCL::Bindings {

///
/// \brief The Scene class.
///
class Scene {
public:
    ///
    /// \brief Shared pointer alias
    ///
    using SharedPtr = std::shared_ptr<Scene>;

    ///
    /// \brief Returns all materials of the scene.
    /// \return Materials used in scene.
    ///
    std::vector<Material::SharedPtr> getMaterials();

    ///
    /// \brief Append a material to the scene.
    /// \param material Material used in scene.
    ///
    void addMaterial(const Material::SharedPtr& material);

    ///
    /// \brief Returns all models of the scene.
    /// \return Models of the scene.
    ///
    std::vector<Model::SharedPtr> getModels();

    ///
    /// \brief Append a model to the scene.
    /// \param Model to be added to the scene.
    ///
    void addModel(const Model::SharedPtr& model);

    ///
    /// \brief Returns all animations of the scene.
    /// \return Animations of the scene.
    ///
    std::vector<Animation::SharedPtr> getAnimations();

    ///
    /// \brief Append an animation to the scene.
    /// \param Animation to be added to the scene.
    ///
    void addAnimation(const Animation::SharedPtr& animation);

    ///
    /// \brief Returns all imported file paths of the scene.
    /// \return Imported files paths
    ///
    std::vector<std::string> getImportedFilePaths();

    ///
    /// \brief Append a imported file path to the scene.
    /// \param importedFilePath Imported file path to be added.
    ///
    void addImportedFilePath(const std::string& importedFilePath);

    ///
    /// \brief Returns all search paths of the scene.
    /// \return Search paths to look up for scene relevant files e.g. textures.
    ///
    std::set<std::string> getSearchPaths();

    ///
    /// \brief Append a search path to the scene.
    /// \param searchPath Search path to be added.
    ///
    void addSearchPath(const std::string& searchPath);

protected:
    ///
    /// \brief Models of the scene.
    ///
    std::vector<Model::SharedPtr> m_models;

    ///
    /// \brief Materials of the scene.
    ///
    std::vector<Material::SharedPtr> m_materials;

    ///
    /// \brief Animations of the scene.
    ///
    std::vector<Animation::SharedPtr> m_animations;

    ///
    /// \brief Imported file paths of the scene.
    ///
    std::vector<std::string> m_importedFilePaths;

    ///
    /// \brief Search paths to look up for scene relevant files e.g. textures.
    ///
    std::set<std::string> m_searchPaths;
};

}  // namespace GCL::Bindings
