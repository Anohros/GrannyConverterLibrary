#include "gcl/exporter/fbxexporterskeleton.h"

namespace GCL::Exporter {

void FbxExporterSkeleton::exportBones(Model::SharedPtr model)
{
    for (auto bone : model->getBones()) {
        exportBone(model, bone);
    }
}

void FbxExporterSkeleton::exportBone(Model::SharedPtr model, Bone::SharedPtr bone)
{
    auto grannyBone = bone->getData();
    auto parentIndex = grannyBone.ParentIndex;

    // Set bone transformation.
    auto localTransform = bone->getData().LocalTransform;
    auto boneTransform = FbxAMatrix(
        FbxDouble3(0, 0, 0),
        FbxDouble3(0, 0, 0),
        FbxDouble3(1, 1, 1));

    if (localTransform.Flags & GrannyTransformFlags::GrannyHasPosition) {
        boneTransform.SetT(FbxVector4(
            static_cast<double>(localTransform.Position[0]),
            static_cast<double>(localTransform.Position[1]),
            static_cast<double>(localTransform.Position[2])));
    }

    if (localTransform.Flags & GrannyTransformFlags::GrannyHasOrientation) {
        boneTransform.SetQ(FbxQuaternion(
            static_cast<double>(localTransform.Orientation[0]),
            static_cast<double>(localTransform.Orientation[1]),
            static_cast<double>(localTransform.Orientation[2]),
            static_cast<double>(localTransform.Orientation[3])));
    }

    if (localTransform.Flags & GrannyTransformFlags::GrannyHasScaleShear) {
        boneTransform.SetS(FbxDouble3(
            static_cast<double>(localTransform.ScaleShear[0][0]),
            static_cast<double>(localTransform.ScaleShear[1][1]),
            static_cast<double>(localTransform.ScaleShear[2][2])));
    }

    // Multiply bone transformation by initial model placement.
    if (parentIndex == GrannyNoParentBone) {
        auto initialPlacement = model->getData()->InitialPlacement;
        auto initialTransform = FbxAMatrix(
            FbxDouble3(0, 0, 0),
            FbxDouble3(0, 0, 0),
            FbxDouble3(1, 1, 1));

        if (initialPlacement.Flags & GrannyTransformFlags::GrannyHasPosition) {
            initialTransform.SetT(FbxDouble3(
                static_cast<double>(initialPlacement.Position[0]),
                static_cast<double>(initialPlacement.Position[1]),
                static_cast<double>(initialPlacement.Position[2])));
        }

        if (initialPlacement.Flags & GrannyTransformFlags::GrannyHasOrientation) {
            initialTransform.SetQ(FbxQuaternion(
                static_cast<double>(initialPlacement.Orientation[0]),
                static_cast<double>(initialPlacement.Orientation[1]),
                static_cast<double>(initialPlacement.Orientation[2]),
                static_cast<double>(initialPlacement.Orientation[3])));
        }

        if (initialPlacement.Flags & GrannyTransformFlags::GrannyHasScaleShear) {
            initialTransform.SetS(FbxDouble3(
                static_cast<double>(initialPlacement.ScaleShear[0][0]),
                static_cast<double>(initialPlacement.ScaleShear[1][1]),
                static_cast<double>(initialPlacement.ScaleShear[2][2])));
        }

        boneTransform = initialTransform * boneTransform;
    }

    // Setup the bone node.
    auto boneName = grannyBone.Name;
    auto boneNode = FbxNode::Create(m_fbxScene, boneName);
    boneNode->LclTranslation.Set(boneTransform.GetT());
    boneNode->LclRotation.Set(boneTransform.GetR());
    boneNode->LclScaling.Set(boneTransform.GetS());
    bone->setNode(boneNode);

    // Set inverse bone transformation.
    FbxAMatrix world;
    world.SetRow(0, FbxVector4(reinterpret_cast<double*>(grannyBone.InverseWorld4x4[0])));
    world.SetRow(1, FbxVector4(reinterpret_cast<double*>(grannyBone.InverseWorld4x4[1])));
    world.SetRow(2, FbxVector4(reinterpret_cast<double*>(grannyBone.InverseWorld4x4[2])));
    world.SetRow(3, FbxVector4(reinterpret_cast<double*>(grannyBone.InverseWorld4x4[3])));
    boneNode->SetGeometricTranslation(FbxNode::EPivotSet::eDestinationPivot, world.GetT());
    boneNode->SetGeometricRotation(FbxNode::EPivotSet::eDestinationPivot, world.GetR());
    boneNode->SetGeometricScaling(FbxNode::EPivotSet::eDestinationPivot, world.GetS());

    // Create the skeleton of this bone for the fbx scene.
    auto skeleton = FbxSkeleton::Create(m_fbxScene, boneName);
    boneNode->SetNodeAttribute(skeleton);

    // Add the bone as root or child bone depending on if granny bone has a parent.
    if (parentIndex == GrannyNoParentBone) {
        // Set skeleton type to root. Root bones are visible.
        skeleton->SetSkeletonType(FbxSkeleton::eRoot);

        // Add bone node as root to the fbx scene.
        m_fbxScene->GetRootNode()->AddChild(boneNode);
    } else if (parentIndex > -1 && parentIndex < static_cast<int>(model->getBones().size())) {
        // Set skeleton type to limb node. Limb bones are visible.
        skeleton->SetSkeletonType(FbxSkeleton::eLimbNode);

        // Add the bone node as child node of the parent node.
        auto parentBoneNode = model->getBones().at(static_cast<unsigned>(parentIndex))->getNode();
        parentBoneNode->AddChild(boneNode);
    }
}

void FbxExporterSkeleton::exportPoses(Model::SharedPtr model)
{
    exportBindPose(model);
    exportRestPose(model);
}

void FbxExporterSkeleton::exportBindPose(Model::SharedPtr model)
{
    FbxNode* rootBone = model->getBones().at(0)->getNode();

    auto bindPose = FbxPose::Create(m_fbxScene, "BindPose");
    bindPose->SetIsBindPose(true);

    for (auto mesh : model->getMeshes()) {
        if (!mesh->isExcluded() && mesh->getNode()) {
            bindPose->Add(mesh->getNode(), mesh->getNode()->EvaluateGlobalTransform());
        }
    }

    bindPose->Add(rootBone, rootBone->EvaluateGlobalTransform());
    m_fbxScene->AddPose(bindPose);
}

void FbxExporterSkeleton::exportRestPose(Model::SharedPtr model)
{
    FbxNode* rootBone = model->getBones().at(0)->getNode();

    auto restPose = FbxPose::Create(m_fbxScene, "RestPose");
    restPose->SetIsBindPose(false);

    FbxMatrix restPoseMatrix;
    FbxVector4
        restPoseTransform,
        restPoseRotation,
        restPoseScale(1.0, 1.0, 1.0);

    restPoseMatrix.SetTRS(restPoseTransform, restPoseRotation, restPoseScale);

    for (auto mesh : model->getMeshes()) {
        if (!mesh->isExcluded() && mesh->getNode()) {
            restPose->Add(mesh->getNode(), restPoseMatrix);
        }
    }

    restPose->Add(rootBone, rootBone->EvaluateGlobalTransform());
    m_fbxScene->AddPose(restPose);
}

} // namespace GCL::Exporter
