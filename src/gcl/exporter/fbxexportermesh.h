#pragma once

#include <map>
#include <vector>

#include "gcl/exporter/fbxexportermodule.h"
#include "gcl/utilities/fbxsdkcommon.h"
#include "gcl/utilities/materialutility.h"

namespace GCL::Exporter {

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
    virtual ~FbxExporterMesh() = default;

    ///
    /// \brief Export the meshes of the given model to the fbx scene.
    /// \param model Model of which the meshes need to be exported of to the fbx scene.
    /// \param export_skeleton
    ///
    void ExportMeshes(const Bindings::Model::SharedPtr& model, bool export_skeleton = false);

protected:
    ///
    /// \brief Export the meshes of the given model to the fbx scene.
    /// \param model A model the mesh is related to.
    /// \param mesh The mesh which needs to be exported.
    /// \param export_skeleton
    ///
    void ExportMesh(
        const Bindings::Model::SharedPtr& model,
        const Bindings::Mesh::SharedPtr& mesh,
        bool export_skeleton = false
    );

    ///
    /// \brief Applies the bone weights and bone deformation for a mesh.
    /// \param model A model the mesh is related to.
    /// \param mesh The mesh which needs the bone weights and deformation to be applied.
    /// \param mesh_node The fbx node of the mesh.
    /// \param fbx_mesh The fbx mesh of the mesh.
    ///
    void CreateBoneWeightsAndApplyDeformation(
        const Bindings::Model::SharedPtr& model,
        const Bindings::Mesh::SharedPtr& mesh,
        FbxNode* mesh_node,
        FbxMesh* fbx_mesh
    );

    ///
    /// \brief Maps bone names to their corresponding shared pointer bindings.
    ///
    using BoneMap = std::map<std::string, Bindings::Bone::SharedPtr>;

    ///
    /// \brief A collection of shared pointers to bone bindings, ordered by hierarchy.
    ///
    using BoneBindingList = std::vector<Bindings::BoneBinding::SharedPtr>;

    // Wrapper for all bones available in the scene mesh.
    struct AllBones {
        BoneMap data;
    };

    // Wrapper for bones that have already been bound during export.
    struct BoundBones {
        BoneMap data;
    };

    ///
    /// \brief Initializes bone bindings for a mesh.
    /// \param model The model containing bones.
    /// \param mesh The mesh to process.
    /// \param all_bones Map of bone names to bone objects.
    /// \param bound_bones Map of already bound bones.
    /// \param bone_bindings Vector to store bone bindings.
    ///
    void InitializeBoneBindings(
        const Bindings::Model::SharedPtr& model,
        const Bindings::Mesh::SharedPtr& mesh,
        AllBones* all_bones,
        BoundBones* bound_bones,
        BoneBindingList* bone_bindings
    ) const;

    ///
    /// \brief Applies bone weights for rigid meshes.
    /// \param mesh The mesh to process.
    /// \param bone_bindings Vector of bone bindings.
    /// \param vertices The mesh vertices.
    ///
    static void ApplyRigidBoneWeights(
        const Bindings::Mesh::SharedPtr& mesh,
        const BoneBindingList& bone_bindings,
        const std::vector<GrannyPWNT34322Vertex>& vertices
    );

    ///
    /// \brief Applies bone weights for non-rigid meshes.
    /// \param mesh The mesh to process.
    /// \param bone_bindings Vector of bone bindings.
    /// \param vertices The mesh vertices.
    ///
    static void ApplyNonRigidBoneWeights(
        const Bindings::Mesh::SharedPtr& mesh,
        const BoneBindingList& bone_bindings,
        const std::vector<GrannyPWNT34322Vertex>& vertices
    );

    ///
    /// \brief Maps bones without bone binding.
    /// \param mesh The mesh to process.
    /// \param all_bones Map of bone names to bone objects.
    /// \param bound_bones Map of already bound bones.
    /// \param bone_bindings Vector to store bone bindings.
    /// \param vertices The mesh vertices.
    ///
    void MapUnboundBones(
        const AllBones* all_bones,
        const BoundBones* bound_bones,
        const std::vector<GrannyPWNT34322Vertex>& vertices,
        BoneBindingList* bone_bindings
    );

    ///
    /// \brief Creates the bone deformation for a mesh.
    /// \param mesh_node The fbx node of the mesh.
    /// \param mesh The fbx mesh of the mesh.
    /// \param bone_bindings The bone bindings of the mesh.
    /// \param is_rigid
    ///
    void CreateMeshDeformation(
        FbxNode* mesh_node,
        FbxMesh* mesh,
        const BoneBindingList& bone_bindings,
        bool is_rigid = false
    );

    ///
    /// \brief Export a mesh to a fbx mesh.
    /// \param mesh The mesh which needs to be exported as fbx mesh.
    /// \return Fbx mesh variant of mesh which needed to be exported as fbx mehs.
    ///
    FbxMesh* ExportFbxMesh(const Bindings::Mesh::SharedPtr& mesh);

    ///
    /// \brief Creates the control points for a mesh from its vertices.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    static void CreateControlPoints(
        FbxMesh* mesh, const std::vector<GrannyPWNT34322Vertex>& vertices
    );

    ///
    /// \brief Create unique material geometry element for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    ///
    static void CreateMaterial(FbxMesh* mesh);

    ///
    /// \brief Create unique normal geometry element for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.static
    ///
    static void CreateNormal(FbxMesh* mesh, const std::vector<GrannyPWNT34322Vertex>& vertices);

    ///
    /// \brief Counts the number of texture coordinate types in a mesh vertex data.
    /// \param mesh The mesh to analyze.
    /// \return Number of texture coordinate types found.
    ///
    static int CountTextureCoordinateTypes(const Bindings::Mesh::SharedPtr& mesh);

    ///
    /// \brief Create uv geometry elements (uv-sets) for the mesh.
    /// \param mesh The mesh which need its uv-set to be created.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    static void CreateUV(
        const Bindings::Mesh::SharedPtr& mesh,
        FbxMesh* fbx_mesh,
        const std::vector<GrannyPWNT34322Vertex>& vertices
    );

    ///
    /// \brief Create element uv1 for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    static void CreateUV1(FbxMesh* fbx_mesh, const std::vector<GrannyPWNT34322Vertex>& vertices);

    ///
    /// \brief Create element uv2 for the mesh.
    /// \param fbxMesh The fbx mesh of the mesh.
    /// \param vertices The vertices of the mesh.
    ///
    static void CreateUV2(FbxMesh* fbx_mesh, const std::vector<GrannyPWNT34322Vertex>& vertices);

    ///
    /// \brief Binds the materials from the scene to a mesh.
    /// \param mesh The mesh which need its materials to be binded.
    ///
    void BindMaterials(const Bindings::Mesh::SharedPtr& mesh);

    ///
    /// \brief Returns the material index for a geometry topological index.
    /// \param mesh The mesh which the geometry topological index is related to.
    /// \param index The geometry topological index of the related mesh.
    ///
    int GetMaterialForIndex(const Bindings::Mesh::SharedPtr& mesh, size_t index);

    /// Finds the matching native FBX surface material in the global scene list.
    ///
    /// Iterates through all loaded materials in the scene and matches them
    /// against the provided Granny material data source.
    ///
    /// \param target_material The source Granny material to look for.
    /// \return A pointer to the FBX material node, or nullptr if not found.
    FbxSurfaceMaterial* FindSceneMaterial(const GrannyMaterial* target_material) const;

    /// Determines the local material index of an FBX material attached to a node.
    ///
    /// \param mesh_node The FBX node where the material is attached.
    /// \param target_material The FBX material whose index is required.
    /// \return The zero-based index of the material on the node, or -1 if missing
    static int FindMaterialIndexAtNode(
        FbxNode* mesh_node, const FbxSurfaceMaterial* target_material
    );

    ///
    /// \brief Sanitizes a material name.
    /// \param name
    /// \return Sanitized name of the material.
    ///
    virtual std::string SanitizeMaterialName(std::string name);
};

}  // namespace GCL::Exporter
