#include "gcl/importer/grannyimporterskeleton.h"

namespace GCL::Importer {

GrannyImporterSkeleton::GrannyImporterSkeleton(Scene::SharedPtr scene)
    : m_scene(scene)
{
}

GrannyImporterSkeleton::~GrannyImporterSkeleton()
{
}

vector<Bone::SharedPtr> GrannyImporterSkeleton::loadBones(GrannyModel* grannyModel) const
{
    unsigned boneCount = static_cast<unsigned>(grannyModel->Skeleton->BoneCount);

    vector<Bone::SharedPtr> bones;
    bones.reserve(boneCount);

    // Import each bone of the granny model as scene bone.
    for (unsigned i = 0; i < boneCount; i++) {
        bones.push_back(make_shared<Bone>(grannyModel->Skeleton->Bones[i]));
    }

    return bones;
}

} // namespace GCL::Importer
