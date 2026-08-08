#include "gcl/bindings/scene.h"

namespace GCL::Bindings {

std::vector<Material::SharedPtr> Scene::getMaterials() {
    return m_materials;
}

void Scene::addMaterial(const Material::SharedPtr& material) {
    m_materials.push_back(material);
}

std::vector<Model::SharedPtr> Scene::getModels() {
    return m_models;
}

void Scene::addModel(const Model::SharedPtr& model) {
    m_models.push_back(model);
}

std::vector<Animation::SharedPtr> Scene::getAnimations() {
    return m_animations;
}

void Scene::addAnimation(const Animation::SharedPtr& animation) {
    m_animations.push_back(animation);
}

std::vector<std::string> Scene::getImportedFilePaths() {
    return m_importedFilePaths;
}

void Scene::addImportedFilePath(const std::string& importedFilePath) {
    m_importedFilePaths.push_back(importedFilePath);
}

std::set<std::string> Scene::getSearchPaths() {
    return m_searchPaths;
}

void Scene::addSearchPath(const std::string& searchPath) {
    m_searchPaths.insert(searchPath);
}

}  // namespace GCL::Bindings
