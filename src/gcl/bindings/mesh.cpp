#include "gcl/bindings/mesh.h"

namespace GCL::Bindings {

Mesh::Mesh(GrannyMesh* data) : m_data(data) {
}

GrannyMesh* Mesh::getData() {
    return m_data;
}

FbxNode* Mesh::getNode() {
    return m_node;
}

std::vector<BoneBinding::SharedPtr> Mesh::getBoneBindings() {
    return m_boneBindings;
}

void Mesh::setData(GrannyMesh* data) {
    m_data = data;
}

void Mesh::setNode(FbxNode* node) {
    m_node = node;
}

void Mesh::addBoneBinding(const BoneBinding::SharedPtr& binding) {
    m_boneBindings.push_back(binding);
}

bool Mesh::isRigid() {
    return GrannyMeshIsRigid(m_data);
}

std::vector<GrannyPWNT34322Vertex> Mesh::getRigidVertices() {
    const size_t vertex_count = static_cast<size_t>(GrannyGetMeshVertexCount(m_data));
    if (vertex_count == 0) {
        return {};
    }
    std::vector<GrannyPWNT34322Vertex> vertices(vertex_count);
    GrannyCopyMeshVertices(m_data, GrannyPWNT34322VertexType, vertices.data());
    return vertices;
}

}  // namespace GCL::Bindings
