#include "gcl/bindings/mesh.h"

namespace GCL::Bindings {

Mesh::Mesh(GrannyMesh* data)
    : m_data(data)
    , m_node(nullptr)
{
}

GrannyMesh* Mesh::getData()
{
    return m_data;
}

FbxNode* Mesh::getNode()
{
    return m_node;
}

vector<BoneBinding::SharedPtr> Mesh::getBoneBindings()
{
    return m_boneBindings;
}

void Mesh::setData(GrannyMesh* data)
{
    m_data = data;
}

void Mesh::setNode(FbxNode* node)
{
    m_node = node;
}

void Mesh::addBoneBinding(BoneBinding::SharedPtr binding)
{
    m_boneBindings.push_back(binding);
}

bool Mesh::isRigid()
{
    return GrannyMeshIsRigid(m_data);
}

vector<GrannyPWNT3432Vertex> Mesh::getRigidVertices()
{
    unsigned grannyVertexCount = static_cast<unsigned>(GrannyGetMeshVertexCount(m_data));
    GrannyPWNT3432Vertex* grannyVertices = new GrannyPWNT3432Vertex[grannyVertexCount];

    vector<GrannyPWNT3432Vertex> rigidVertices;
    rigidVertices.reserve(grannyVertexCount);

    GrannyCopyMeshVertices(m_data, GrannyPWNT3432VertexType, grannyVertices);

    for (unsigned i = 0; i < grannyVertexCount; i++) {
        rigidVertices.push_back(grannyVertices[i]);
    }

    delete[] grannyVertices;

    return rigidVertices;
}

} // namespace GCL::Bindings
