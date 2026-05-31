#include "gcl/bindings/model.h"

#include <algorithm>
#include <utility>

namespace GCL::Bindings {

Model::Model(GrannyModel* data) : m_data(data) {
}

GrannyModel* Model::getData() {
    return m_data;
}

std::vector<Mesh::SharedPtr> Model::getMeshes() {
    return m_meshes;
}

std::vector<Bone::SharedPtr> Model::getBones() {
    return m_bones;
}

void Model::setMeshes(std::vector<Mesh::SharedPtr> meshes) {
    m_meshes.swap(meshes);
}

void Model::setBones(std::vector<Bone::SharedPtr> bones) {
    m_bones = std::move(bones);
}

bool Model::hasRigidMeshes() const {
    return std::any_of(m_meshes.begin(), m_meshes.end(), [](const Mesh::SharedPtr& mesh) {
        return mesh->isRigid();
    });
}

void Model::setTransform(const FbxMatrix& transform) {
    m_transform = transform;
}

}  // namespace GCL::Bindings
