#include "gcl/importer/grannyimporterskeleton.h"

namespace GCL::Importer {

GrannyImporterSkeleton::GrannyImporterSkeleton(Bindings::Scene::SharedPtr scene) : m_scene(scene) {
}

std::vector<Bindings::Bone::SharedPtr> GrannyImporterSkeleton::loadBones(
    GrannyModel* grannyModel
) const {
    unsigned boneCount = static_cast<unsigned>(grannyModel->Skeleton->BoneCount);

    std::vector<Bindings::Bone::SharedPtr> bones;
    bones.reserve(boneCount);

    // Import each bone of the granny model as scene bone.
    for (unsigned i = 0; i < boneCount; i++) {
        bones.push_back(std::make_shared<Bindings::Bone>(grannyModel->Skeleton->Bones[i]));
    }

    return bones;
}

}  // namespace GCL::Importer
