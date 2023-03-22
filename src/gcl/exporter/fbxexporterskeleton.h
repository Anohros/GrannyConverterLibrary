#pragma once

#include "gcl/exporter/fbxexportermodule.h"

namespace GCL::Exporter {

using namespace std;
using namespace GCL::Bindings;

///
/// \brief The ExporterSkeleton class.
///
class FbxExporterSkeleton : public FbxExporterModule {
public:
    // Inherit constructor.
    using FbxExporterModule::FbxExporterModule;

    ///
    /// \brief Export the bones of a model to the fbx scene.
    /// \param model Model of which the bones need to be exported of to the fbx scene.
    ///
    void exportBones(Model::SharedPtr model);

    ///
    /// \brief Export the poses of a model to the fbx scene.
    /// \param model Model of which the poses need to be exported of to the fbx scene.
    ///
    void exportPoses(Model::SharedPtr model);

protected:
    ///
    /// \brief Export a bone of the scene to the fbx scene.
    /// \param model A model the bone is related to.
    /// \param bone A bone which needs to be exported.
    ///
    void exportBone(Model::SharedPtr model, Bone::SharedPtr bone);

    ///
    /// \brief Export the bind pose of a model to the fbx scene.
    /// \param model Model of which the bind pose needs to be exported of to the fbx scene.
    ///
    void exportBindPose(Model::SharedPtr model);

    ///
    /// \brief Expands a bone cluster to list bone clusters recursively.
    /// \param boneClusters boneClusters
    /// \param boneCluster boneCluster
    ///
    void expandBoneCluster(vector<FbxNode*>& boneClusters, FbxNode* boneCluster);

    ///
    /// \brief Export the rest pose of a model to the fbx scene.
    /// \param model Model of which the rest pose needs to be exported of to the fbx scene.
    ///
    void exportRestPose(Model::SharedPtr model);
};

} // namespace GCL::Exporter
