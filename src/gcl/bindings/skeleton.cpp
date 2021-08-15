#include "gcl/bindings/skeleton.h"

namespace GCL::Bindings {

Skeleton::Skeleton(GrannySkeleton* data)
    : m_data(data)
    , m_node(nullptr)
{
}

GrannySkeleton* Skeleton::getData()
{
    return m_data;
}

FbxNode* Skeleton::getNode()
{
    return m_node;
}

void Skeleton::setData(GrannySkeleton* data)
{
    m_data = data;
}

void Skeleton::setNode(FbxNode* node)
{
    m_node = node;
}

vector<Bone::SharedPtr> Skeleton::getBones()
{
    return m_bones;
}

void Skeleton::setBones(vector<Bone::SharedPtr> bones)
{
    m_bones.swap(bones);
}

} // namespace GCL::Bindings
