#pragma once

#include "gcl/exporter/fbxexportermodule.h"
#include "gcl/utilities/fbxsdkcommon.h"
#include "gcl/utilities/materialutility.h"

namespace GCL::Exporter {

using namespace std;
using namespace GCL::Bindings;
using namespace GCL::Utilities;

///
/// \brief The FbxExporterMesh class.
///
class FbxExporterMesh : public FbxExporterModule {
public:
    // Inherit constructor.
    using FbxExporterModule::FbxExporterModule;

    ///
    /// \brief Destructor
    ///
    virtual ~FbxExporterMesh() { }

    ///
    /// \brief Export the meshes of the given model to the fbx scene.
    /// \param model Model of which the meshes need to be exported of to the fbx scene.
    /// \param exportSkeleton
    ///
    void exportMeshes(Model::SharedPtr model, bool exportSkeleton = false);

protected:
    ///
    /// \brief Export the meshes of the given model to the fbx scene.
    /// \param model A model the mesh is related to.
    /// \param mesh The mesh which needs to be exported.
    /// \param exportSkeleton
    ///
    void exportMesh(Model::SharedPtr model, Mesh::SharedPtr mesh, bool exportSkeleton = false);

    ///
    /// \brief Creates the bone deformation for a mesh.
    /// \param meshNode The fbx node of the mesh.
    /// \param mesh The fbx mesh of the mesh.
    /// \param boneBindings The bone bindings of the mesh.
    ///
    void createMeshDeformation(FbxNode* meshNode, FbxMesh* mesh, vector<BoneBinding::SharedPtr> boneBindings);

    ///
    /// \brief Applies the bone weights and bone deformation for a mesh.
    /// \param model A model the mesh is related to.
    /// \param mesh The mesh which needs the bone weights and deformation to be applied.
    /// \param meshNode The fbx node of the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    ///
    void createBoneWeightsAndApplyDeformation(Model::SharedPtr model, Mesh::SharedPtr mesh, FbxNode* meshNode, FbxMesh* fbxMesh);

    ///
    /// \brief Export a mesh to a fbx mesh.
    /// \param mesh The mesh which needs to be exported as fbx mesh.
    /// \return Fbx mesh variant of mesh which needed to be exported as fbx mehs.
    ///
    FbxMesh* exportFbxMesh(Mesh::SharedPtr mesh);

    ///
    /// \brief Creates the control points for a mesh from its vertices.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    void createControlPoints(FbxMesh* mesh, vector<GrannyPWNT34322Vertex> vertices);

    ///
    /// \brief Create unique material geometry element for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    ///
    void createMaterial(FbxMesh* mesh);

    ///
    /// \brief Create unique normal geometry element for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    void createNormal(FbxMesh* mesh, vector<GrannyPWNT34322Vertex> vertices);

    ///
    /// \brief Create uv geometry elements (uv-sets) for the mesh.
    /// \param mesh The mesh which need its uv-set to be created.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    void createUV(Mesh::SharedPtr mesh, FbxMesh* fbxMesh, vector<GrannyPWNT34322Vertex> vertices);

    ///
    /// \brief Binds the materials from the scene to a mesh.
    /// \param mesh The mesh which need its materials to be binded.
    ///
    void bindMaterials(Mesh::SharedPtr mesh);

    ///
    /// \brief Returns the material index for a geometry topological index.
    /// \param mesh The mesh which the geometry topological index is related to.
    /// \param index The geometry topological index of the related mesh.
    ///
    int getMaterialForIndex(Mesh::SharedPtr mesh, int index);

    ///
    /// \brief Sanitizes a material name.
    /// \param name
    /// \return Sanitized name of the material.
    ///
    virtual string sanitizeMaterialName(string name);
};

} // namespace GCL::Exporter
