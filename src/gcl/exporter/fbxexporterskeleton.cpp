#include "gcl/exporter/fbxexporterskeleton.h"

namespace GCL::Exporter {

void FbxExporterSkeleton::exportBones(const Bindings::Model::SharedPtr& model) {
    for (const auto& bone : model->getBones()) {
        exportBone(model, bone);
    }
}

void FbxExporterSkeleton::exportBone(
    const Bindings::Model::SharedPtr& model, const Bindings::Bone::SharedPtr& bone
) {
    auto granny_bone = bone->getData();
    auto parent_index = granny_bone.ParentIndex;

    // Set bone transformation.
    auto local_transform = bone->getData().LocalTransform;
    auto bone_transform = FbxAMatrix(FbxDouble3(0, 0, 0), FbxDouble3(0, 0, 0), FbxDouble3(1, 1, 1));

    if ((local_transform.Flags & GrannyTransformFlags::GrannyHasPosition) != 0U) {
        bone_transform.SetT(FbxVector4(
            static_cast<double>(local_transform.Position[0]),
            static_cast<double>(local_transform.Position[1]),
            static_cast<double>(local_transform.Position[2])
        ));
    }

    if ((local_transform.Flags & GrannyTransformFlags::GrannyHasOrientation) != 0U) {
        bone_transform.SetQ(FbxQuaternion(
            static_cast<double>(local_transform.Orientation[0]),
            static_cast<double>(local_transform.Orientation[1]),
            static_cast<double>(local_transform.Orientation[2]),
            static_cast<double>(local_transform.Orientation[3])
        ));
    }

    if ((local_transform.Flags & GrannyTransformFlags::GrannyHasScaleShear) != 0U) {
        bone_transform.SetS(FbxDouble3(
            static_cast<double>(local_transform.ScaleShear[0][0]),
            static_cast<double>(local_transform.ScaleShear[1][1]),
            static_cast<double>(local_transform.ScaleShear[2][2])
        ));
    }

    // Multiply bone transformation by initial model placement.
    if (parent_index == GrannyNoParentBone) {
        auto initial_placement = model->getData()->InitialPlacement;
        auto initial_transform =
            FbxAMatrix(FbxDouble3(0, 0, 0), FbxDouble3(0, 0, 0), FbxDouble3(1, 1, 1));

        if ((initial_placement.Flags & GrannyTransformFlags::GrannyHasPosition) != 0U) {
            initial_transform.SetT(FbxDouble3(
                static_cast<double>(initial_placement.Position[0]),
                static_cast<double>(initial_placement.Position[1]),
                static_cast<double>(initial_placement.Position[2])
            ));
        }

        if ((initial_placement.Flags & GrannyTransformFlags::GrannyHasOrientation) != 0U) {
            initial_transform.SetQ(FbxQuaternion(
                static_cast<double>(initial_placement.Orientation[0]),
                static_cast<double>(initial_placement.Orientation[1]),
                static_cast<double>(initial_placement.Orientation[2]),
                static_cast<double>(initial_placement.Orientation[3])
            ));
        }

        if ((initial_placement.Flags & GrannyTransformFlags::GrannyHasScaleShear) != 0U) {
            initial_transform.SetS(FbxDouble3(
                static_cast<double>(initial_placement.ScaleShear[0][0]),
                static_cast<double>(initial_placement.ScaleShear[1][1]),
                static_cast<double>(initial_placement.ScaleShear[2][2])
            ));
        }

        bone_transform = initial_transform * bone_transform;
    }

    // Setup the bone node.
    const auto* bone_name = granny_bone.Name;
    auto* bone_node = FbxNode::Create(fbx_scene_, bone_name);
    bone_node->LclTranslation.Set(bone_transform.GetT());
    bone_node->LclRotation.Set(bone_transform.GetR());
    bone_node->LclScaling.Set(bone_transform.GetS());
    bone->setNode(bone_node);

    // Set inverse bone transformation.
    FbxAMatrix world;
    world.SetRow(
        0,
        FbxVector4(
            static_cast<double>(granny_bone.InverseWorld4x4[0][0]),
            static_cast<double>(granny_bone.InverseWorld4x4[0][1]),
            static_cast<double>(granny_bone.InverseWorld4x4[0][2]),
            static_cast<double>(granny_bone.InverseWorld4x4[0][3])
        )
    );
    world.SetRow(
        1,
        FbxVector4(
            static_cast<double>(granny_bone.InverseWorld4x4[1][0]),
            static_cast<double>(granny_bone.InverseWorld4x4[1][1]),
            static_cast<double>(granny_bone.InverseWorld4x4[1][2]),
            static_cast<double>(granny_bone.InverseWorld4x4[1][3])
        )
    );
    world.SetRow(
        2,
        FbxVector4(
            static_cast<double>(granny_bone.InverseWorld4x4[2][0]),
            static_cast<double>(granny_bone.InverseWorld4x4[2][1]),
            static_cast<double>(granny_bone.InverseWorld4x4[2][2]),
            static_cast<double>(granny_bone.InverseWorld4x4[2][3])
        )
    );
    world.SetRow(
        3,
        FbxVector4(
            static_cast<double>(granny_bone.InverseWorld4x4[3][0]),
            static_cast<double>(granny_bone.InverseWorld4x4[3][1]),
            static_cast<double>(granny_bone.InverseWorld4x4[3][2]),
            static_cast<double>(granny_bone.InverseWorld4x4[3][3])
        )
    );
    bone_node->SetGeometricTranslation(FbxNode::EPivotSet::eDestinationPivot, world.GetT());
    bone_node->SetGeometricRotation(FbxNode::EPivotSet::eDestinationPivot, world.GetR());
    bone_node->SetGeometricScaling(FbxNode::EPivotSet::eDestinationPivot, world.GetS());

    // Create the skeleton of this bone for the fbx scene.
    auto* skeleton = FbxSkeleton::Create(fbx_scene_, bone_name);
    bone_node->SetNodeAttribute(skeleton);

    // Add the bone as root or child bone depending on if granny bone has a parent.
    if (parent_index == GrannyNoParentBone) {
        // Set skeleton type to root. Root bones are visible.
        skeleton->SetSkeletonType(FbxSkeleton::eRoot);

        // Add bone node as root to the fbx scene.
        fbx_scene_->GetRootNode()->AddChild(bone_node);
    } else if (parent_index > -1 && parent_index < model->getBones().size()) {
        // Set skeleton type to limb node. Limb bones are visible.
        skeleton->SetSkeletonType(FbxSkeleton::eLimbNode);

        // Add the bone node as child node of the parent node.
        auto* parent_bone_node = model->getBones().at(parent_index)->getNode();
        parent_bone_node->AddChild(bone_node);
    }
}

void FbxExporterSkeleton::exportPoses(const Bindings::Model::SharedPtr& model) {
    exportBindPose(model);
    exportRestPose(model);
}

void FbxExporterSkeleton::exportBindPose(const Bindings::Model::SharedPtr& model) {
    FbxNode* root_bone = model->getBones().at(0)->getNode();
    std::vector<FbxNode*> bone_clusters;

    if (root_bone != nullptr && root_bone->GetNodeAttribute() != nullptr) {
        FbxGeometry* mesh_geometry = nullptr;
        int mesh_skin_count = 0;
        int bone_cluster_count = 0;

        switch (root_bone->GetNodeAttribute()->GetAttributeType()) {
            default:
                break;
            case FbxNodeAttribute::eMesh:
            case FbxNodeAttribute::eNurbs:
            case FbxNodeAttribute::ePatch:
                mesh_geometry = static_cast<FbxGeometry*>(root_bone->GetNodeAttribute());
                mesh_skin_count = mesh_geometry->GetDeformerCount(FbxDeformer::eSkin);
                for (int mesh_skin_index = 0; mesh_skin_index < mesh_skin_count;
                     mesh_skin_index++) {
                    FbxSkin* mesh_skin = static_cast<FbxSkin*>(
                        mesh_geometry->GetDeformer(mesh_skin_index, FbxDeformer::eSkin)
                    );
                    bone_cluster_count += mesh_skin->GetClusterCount();
                }
                break;
        }

        // If we found some clusters we must expand the node.
        if (bone_cluster_count != 0) {
            for (auto mesh_skin_index = 0; mesh_skin_index < mesh_skin_count; mesh_skin_index++) {
                auto* mesh_skin = static_cast<FbxSkin*>(
                    mesh_geometry->GetDeformer(mesh_skin_index, FbxDeformer::eSkin)
                );
                bone_cluster_count = mesh_skin->GetClusterCount();
                for (auto bone_cluster_index = 0; bone_cluster_index < bone_cluster_count;
                     bone_cluster_index++) {
                    auto* bone_cluster = mesh_skin->GetCluster(bone_cluster_index)->GetLink();
                    expandBoneCluster(bone_clusters, bone_cluster);
                }
            }
            bone_clusters.push_back(root_bone);
        }
    }

    if (!bone_clusters.empty()) {
        auto bind_pose_name = std::string(root_bone->GetName()).append(" BindPose");
        auto* bind_pose = FbxPose::Create(fbx_scene_, bind_pose_name.c_str());
        bind_pose->SetIsBindPose(true);

        for (auto* const bone_cluster : bone_clusters) {
            bind_pose->Add(bone_cluster, bone_cluster->EvaluateGlobalTransform());
        }

        fbx_scene_->AddPose(bind_pose);
    }
}

void FbxExporterSkeleton::expandBoneCluster(
    std::vector<FbxNode*>& boneClusters, FbxNode* boneCluster
) {
    if (boneCluster != nullptr) {
        expandBoneCluster(boneClusters, boneCluster->GetParent());

        if (find(boneClusters.begin(), boneClusters.end(), boneCluster) == boneClusters.end()) {
            boneClusters.push_back(boneCluster);
        }
    }
}

void FbxExporterSkeleton::exportRestPose(const Bindings::Model::SharedPtr& model) {
    auto* root_bone = model->getBones().at(0)->getNode();
    auto rest_pose_name = std::string(root_bone->GetName()).append(" RestPose");
    auto* rest_pose = FbxPose::Create(fbx_scene_, rest_pose_name.c_str());
    rest_pose->SetIsBindPose(false);
    FbxMatrix rest_pose_matrix;
    FbxVector4 rest_pose_transform;
    FbxVector4 rest_pose_rotation;
    FbxVector4 rest_pose_scale(1.0, 1.0, 1.0);
    rest_pose_matrix.SetTRS(rest_pose_transform, rest_pose_rotation, rest_pose_scale);
    rest_pose->Add(root_bone, rest_pose_matrix, true);
    fbx_scene_->AddPose(rest_pose);
}

}  // namespace GCL::Exporter
