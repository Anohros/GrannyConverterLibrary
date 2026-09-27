#pragma once

#include <Windows.h>
#include <string>

#pragma pack(push, 4)

// Granny specific data structures.

typedef float GrannyTriple[3];
typedef float GrannyQuad[4];
typedef float GrannyMatrix4x4[4][4];

///
/// \brief Stores data of pixel layout.
///
struct GrannyPixelLayout {
    int bytes_per_pixel;
    int shift_for_component[4];
    int bits_for_component[4];
};

///
/// \brief Stores of data of mip level of a texture image.
///
struct GrannyTextureMIPLevel {
    int stride;
    int pixel_byte_count;
    void* pixel_bytes;
};

///
/// \brief Stores details for a texture image.
///
struct GrannyTextureImage {
    int mip_level_count;
    GrannyTextureMIPLevel* mip_levels;
};

///
/// \brief Stores a variant of data structure.
///
struct GrannyVariant {
    void* type;
    void* object;
};

///
/// \brief Member types for custom data type definitions.
///
enum GrannyMemberType {
    GrannyEndMember,
    GrannyInlineMember,
    GrannyReferenceMember,
    GrannyReferenceToArrayMember,
    GrannyArrayOfReferencesMember,
    GrannyVariantReferenceMember,
    GrannyUnsupportedMemberType_Remove,
    GrannyReferenceToVariantArrayMember,
    GrannyStringMember,
    GrannyTransformMember,
    GrannyReal32Member,
    GrannyInt8Member,
    GrannyUInt8Member,
    GrannyBinormalInt8Member,
    GrannyNormalUInt8Member,
    GrannyInt16Member,
    GrannyUInt16Member,
    GrannyBinormalInt16Member,
    GrannyNormalUInt16Member,
    GrannyInt32Member,
    GrannyUInt32Member,
    GrannyReal16Member,
    GrannyEmptyReferenceMember,
    GrannyOnePastLastMemberType,
    GrannyBool32Member = GrannyInt32Member,
    GrannyMemberTypeForceInt = 0x7fffffff
};

///
/// \brief Defines a custom data type e.g. for extensions.
///
struct GrannyDataTypeDefinition {
    GrannyMemberType type;
    char const* name = "";
    GrannyDataTypeDefinition* reference_type = nullptr;
    int array_width = 0;
    int extra[3] = {0, 0, 0};
    void* ignored_ignored = nullptr;
};

///
/// \brief Stores all data of a texture.
///
/// A texture contains image data with multiple MIP levels and image formats for rendering.
///
struct GrannyTexture {
    char const* from_file_name;
    int texture_type;
    int width;
    int height;
    int encoding;
    int sub_format;
    GrannyPixelLayout layout;
    int image_count;
    GrannyTextureImage* images;
    GrannyVariant extended_data;
};

struct GrannyMaterialMap;

///
/// \brief Stores all data of a material.
///
/// A material defines the visual properties of a mesh including textures, colors, and rendering
/// parameters.
///
struct GrannyMaterial {
    char const* name;
    int map_count;
    GrannyMaterialMap* maps;
    GrannyTexture* texture;
    GrannyVariant extended_data;
};

///
/// \brief Stores a material map.
///
struct GrannyMaterialMap {
    char const* usage;
    GrannyMaterial* material;
};

///
/// \brief Stores a reference to a material.
///
struct GrannyMaterialBinding {
    GrannyMaterial* material;
};

///
/// \brief Stores all transform data of a transformation.
///
struct GrannyTransform {
    unsigned int flags;
    GrannyTriple position;
    GrannyQuad orientation;
    GrannyTriple scale_shear[3];
};

///
/// \brief Stores a variant of animation curve.
///
struct GrannyCurve2 {
    GrannyVariant curve_data;
};

///
/// \brief Stores all data of an animation transform track.
///
struct GrannyTransformTrack {
    char const* name;
    int flags;
    GrannyCurve2 orientation_curve;
    GrannyCurve2 position_curve;
    GrannyCurve2 scale_shear_curve;
};

///
/// \brief Stores all data of an animation track group.
///
struct GrannyTrackGroup {
    char const* name;
    int vector_track_count;
    void* vector_tracks;
    int transform_track_count;
    GrannyTransformTrack* transform_tracks;
    int transform_lod_error_count;
    float* transform_lod_errors;
    int text_track_count;
    void* text_tracks;
    GrannyTransform initial_placement;
    int flags;
    GrannyTriple loop_translation;
    void* periodic_loop;
    GrannyVariant extended_data;
};

///
/// \brief Stores all data of a bone binding.
///
struct GrannyBoneBinding {
    char const* bone_name;
    GrannyTriple obb_min;
    GrannyTriple obb_max;
    int triangle_count;
    int* triangle_indices;
};

///
/// \brief Stores all data of a bone.
///
struct GrannyBone {
    char const* name;
    int parent_index;
    GrannyTransform local_transform;
    GrannyMatrix4x4 inverse_world_4x4;
    float lod_error;
    GrannyVariant extended_data;
};

///
/// \brief Stores all data of a skeleton.
///
/// A skeleton contains a collection of bones that define the hierarchical structure
/// for skinning and animation of 3D models.
///
struct GrannySkeleton {
    char const* name;
    int bone_count;
    GrannyBone* bones;
    int lod_type;
    GrannyVariant extended_data;
};

///
/// \brief Stores all data of a triangle group.
///
struct GrannyTriMaterialGroup {
    int material_index;
    int tri_first;
    int tri_count;
};

///
/// \brief Stores all data of a triangle.
///
struct GrannyTriTopology {
    int group_count;
    GrannyTriMaterialGroup* groups;
    int index_count;
    int* indices;
    int index16_count;
    unsigned short* indices16;
    int vertex_to_vertex_count;
    int* vertex_to_vertex_map;
    int vertex_to_triangle_count;
    int* vertex_to_triangle_map;
    int side_to_neighbor_count;
    unsigned int* side_to_neighbor_map;
    int polygon_index_start_count;
    int* polygon_index_starts;
    int polygon_index_count;
    int* polygon_indices;
    int bones_for_triangle_count;
    int* bones_for_triangle;
    int triangle_to_bone_count;
    int* triangle_to_bone_indices;
    int tri_annotation_set_count;
    void* tri_annotation_sets;
};

///
/// \brief Stores all vertex data of a mesh.
///
/// Vertex data contains the actual vertex information including positions, normals, texture
/// coordinates, and other vertex attributes defined by the vertex type.
///
struct GrannyVertexData {
    GrannyDataTypeDefinition* vertex_type;
    int vertex_count;
    unsigned char* vertices;
    int vertex_component_name_count;
    char const** vertex_component_names;
    int vertex_annotation_set_count;
    void* vertex_annotation_sets;
};

///
/// \brief Stores all data of a mesh.
///
/// A mesh contains vertex data, triangle topology, and material bindings for rendering.
///
struct GrannyMesh {
    char const* name;
    GrannyVertexData* primary_vertex_data;
    int morph_target_count;
    void* morph_targets;
    GrannyTriTopology* primary_topology;
    int material_binding_count;
    GrannyMaterialBinding* material_bindings;
    int bone_binding_count;
    GrannyBoneBinding* bone_bindings;
    GrannyVariant extended_data;
};

///
/// \brief Stores a reference to a mesh also called MeshBinding.
///
struct GrannyModelMeshBinding {
    GrannyMesh* mesh;
};

///
/// \brief Stores all data of a model.
///
/// A model contains a collection of meshes and their bindings to skeletons for animation.
///
struct GrannyModel {
    char const* name;
    GrannySkeleton* skeleton;
    GrannyTransform initial_placement;
    int mesh_binding_count;
    GrannyModelMeshBinding* mesh_bindings;
    GrannyVariant extended_data;
};

///
/// \brief Stores vertex data of a variant of 4 components based vertex structure.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture (UV)
///
struct GrannyPWNT3432Vertex {
    float position[3];
    unsigned char bone_weights[4];
    unsigned char bone_indices[4];
    float normal[3];
    float uv[2];
};

#define GrannyVertexTextureCoordinatesName "TextureCoordinates"

///
/// \brief Defines a 5 components based vertex structure PWNT.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture coordinates for uv channel 1 and 2
///
static GrannyDataTypeDefinition GrannyPWNT34322VertexType[] = {
    {GrannyReal32Member, "Position", 0, 3},
    {GrannyNormalUInt8Member, "BoneWeights", 0, 4},
    {GrannyUInt8Member, "BoneIndices", 0, 4},
    {GrannyReal32Member, "Normal", 0, 3},
    {GrannyReal32Member, GrannyVertexTextureCoordinatesName "0", 0, 2},
    {GrannyReal32Member, GrannyVertexTextureCoordinatesName "1", 0, 2},
    {GrannyEndMember},
};

///
/// \brief Stores vertex data of a variant of 5 components based vertex structure.
///
/// Meaning of PWNT:
///  P: Position
///  W: Weights
///  N: Normal
///  T: Texture coordinates for uv channel 1 (UV1) and 2 (UV2)
///
struct GrannyPWNT34322Vertex {
    float position[3];
    unsigned char bone_weights[4];
    unsigned char bone_indices[4];
    float normal[3];
    float uv1[2];
    float uv2[2];
};

///
/// \brief Stores header of an animation curve like curve format or degree elevation of a curve.
///
struct GrannyCurveDataHeader {
    unsigned char format;
    unsigned char degree;
};

///
/// \brief Stores animation curve data for variant of a keyframe based constant curve.
///
struct GrannyCurveDataDAK32fC32f {
    GrannyCurveDataHeader curve_data_header;
    short padding;
    int knot_count;
    float* knots;
    int control_count;
    float* controls;
};

///
/// \brief Stores all granny curve data formats.
///
enum GrannyCurveDataFormat {
    DaKeyframes32f = 0,
    DaK32fC32f = 1,
    DaIdentity = 2,
    DaConstant32f = 3,
    D3Constant32f = 4,
    D4Constant32f = 5,
    DaK16uC16u = 6,
    DaK8uC8u = 7,
    D4nK16uC15u = 8,
    D4nK8uC7u = 9,
    D3K16uC16u = 10,
    D3K8uC8u = 11,
    D9I1K16uC16u = 12,
    D9I3K16uC16u = 13,
    D9I1K8uC8u = 14,
    D9I3K8uC8u = 15,
    D3I1K32fC32f = 16,
    D3I1K16uC16u = 17,
    D3I1K8uC8u = 18
};

///
/// \brief Stores all data of an animation like name and tracks.
///
struct GrannyAnimation {
    char const* name;
    float duration;
    float time_step;
    float oversampling;
    int track_group_count;
    GrannyTrackGroup** track_groups;
    int default_loop_count;
    int flags;
    GrannyVariant extended_data;
};

#define GrannyNoParentBone -1

///
/// \brief Transform flags
///
enum GrannyTransformFlags {
    GrannyHasPosition = 0x1,
    GrannyHasOrientation = 0x2,
    GrannyHasScaleShear = 0x4,
    GrannyTransformFlagsForceint = 0x7fffffff
};

///
/// \brief Stores all header and data blocks of a granny file.
///
struct GrannyFile {
    void* is_byte_reversed;
    void* header;
    void* source_magic_value;
    int section_count;
    void** sections;
    bool* marshalled;
    bool* is_user_memory;
    void* conversion_buffer;
    unsigned int conversion_buffer_size;
};

///
/// \brief Stores all raw data of a granny file.
///
struct GrannyFileInfo {
    void* art_tool_info;
    void* exporter_info;
    char const* from_file_name;
    int texture_count;
    GrannyTexture** textures;
    int material_count;
    GrannyMaterial** materials;
    int skeleton_count;
    GrannySkeleton** skeletons;
    int vertex_data_count;
    void* vertex_datas;
    int tri_topology_count;
    GrannyTriTopology** tri_topologies;
    int mesh_count;
    GrannyMesh** meshes;
    int model_count;
    GrannyModel** models;
    int track_group_count;
    GrannyTrackGroup** track_groups;
    int animation_count;
    GrannyAnimation** animations;
    GrannyVariant extended_data;
};

///
/// \brief Scene transform flags.
///
enum GrannyTransformFileFlags {
    GrannyRenormalizeNormals = 0x1,
    GrannyReorderTriangleIndices = 0x2,
    Grannytransform_file_flags_forceint = 0x7fffffff
};

// Type definitions for required functions from granny2 dll.

using GrannyReadEntireFile_t = GrannyFile*(__stdcall*)(const char* file_name);
using GrannyGetFileInfo_t = GrannyFileInfo*(__stdcall*)(GrannyFile * file);
using GrannyFreeFile_t = void(__stdcall*)(GrannyFile const* file);
using GrannyGetTotalTypeSize_t = int(__stdcall*)(GrannyDataTypeDefinition* type_definition);
using GrannyGetMeshVertexCount_t = int(__stdcall*)(GrannyMesh const* mesh);
using GrannyCopyMeshVertices_t = void(__stdcall*)(
    GrannyMesh const* mesh, GrannyDataTypeDefinition const* vertex_type, void* dest_vertices
);
using GrannyGetMeshIndexCount_t = int(__stdcall*)(GrannyMesh const* mesh);
using GrannyCopyMeshIndices_t =
    void(__stdcall*)(GrannyMesh const* mesh, int bytes_per_index, void* dest_indices);
using GrannyBuildCompositeTransform4x4_t =
    void(__stdcall*)(GrannyTransform const* transform, float* composite_4x4);
using GrannyMeshIsRigid_t = bool(__stdcall*)(GrannyMesh const* mesh);
using GrannyComputeBasisConversion_t = bool(__stdcall*)(
    GrannyFileInfo const* file_info,
    float desired_units_per_meter,
    float const* desired_origin3,
    float const* desired_right3,
    float const* desired_up3,
    float const* desired_back3,
    float* result_affine3,
    float* result_linear3x3,
    float* result_inverse_linear3x
);
using GrannyTransformFile_t = void(__stdcall*)(
    GrannyFileInfo* file_info,
    float const* affine3,
    float const* linear3x3,
    float const* inverse_linear3x3,
    float affine_tolerance,
    float linear_tolerance,
    unsigned flags
);
using GrannyCurveMakeStaticDaK32fC32f_t = void(__stdcall*)(
    GrannyCurve2* curve,
    GrannyCurveDataDAK32fC32f* curve_data,
    int knot_count,
    int degree,
    int dimension,
    float const* knots,
    float const* controls
);
using GrannyCurveConvertToDaK32fC32f_t = GrannyCurve2*(__stdcall*)(GrannyCurve2 const* src_curve,
                                                                   float const* identity_vector);
using GrannyFreeCurve_t = void(__stdcall*)(GrannyCurve2* curve);
using GrannyCurveGetKnotCount_t = int(__stdcall*)(GrannyCurve2 const* curve);
using GrannyCurveGetDimension_t = int(__stdcall*)(GrannyCurve2 const* curve);
using GrannyCurveGetDegree_t = int(__stdcall*)(GrannyCurve2 const* curve);
using GrannyCurveIdentityPosition_t = float*;
using GrannyCurveIdentityOrientation_t = float*;
using GrannyCurveIdentityScaleShear_t = float*;
using GrannyCurveIdentityScale_t = float*;
using GrannyEvaluateCurveAtT_t = void(__stdcall*)(
    int dimension,
    bool normalize,
    bool backwards_loop,
    GrannyCurve2 const* curve,
    bool forwards_loop,
    float curve_duration,
    float time,
    float* result,
    float const* identity_vector
);
using GrannyEvaluateCurveAtKnotIndex_t = void(__stdcall*)(
    int dimension,
    bool normalize,
    bool backwards_loop,
    GrannyCurve2 const* curve,
    bool forwards_loop,
    float curve_duration,
    int knot_index,
    float time,
    float* result,
    float const* identity_vector
);
using GrannyFindKnot_t = int(__stdcall*)(int knot_count, float* knots, float time);
using GrannyFindCloseKnot_t =
    int(__stdcall*)(int knot_count, float* knots, float time, int starting_index);
using GrannyCurveIsKeyframed_t = bool(__stdcall*)(GrannyCurve2 const* curve);
using GrannyCurveInitializeFormat_t = void(__stdcall*)(GrannyCurve2* curve);
using GrannyCurveDataDaIdentityType_t = GrannyDataTypeDefinition*;
using GrannyTextureHasAlpha_t = bool(__stdcall*)(GrannyTexture const* texture);
using GrannyRGBA8888PixelFormat_t = GrannyPixelLayout*;
using GrannyRGB888PixelFormat_t = GrannyPixelLayout*;
using GrannyCopyTextureImage_t = void(__stdcall*)(
    GrannyTexture const* texture,
    int image_index,
    int mip_index,
    GrannyPixelLayout const* layout,
    int dest_width,
    int dest_height,
    int dest_stride,
    void* pixels
);

#pragma pack(pop)

namespace GCL::Importer {

///
/// \brief Encapsulates the granny2 dll handle and all required function pointers.
///
/// A process-wide singleton is obtained via instance() and initialized once through
/// initialize() (invoked from the GrannyConverterLibrary facade constructor). Every
/// accessor guards against use before initialization: instead of dereferencing a null
/// function pointer it logs a fatal message and returns a safe default value.
///
class GrannyLibrary {
public:
    ///
    /// \brief Returns the process-wide singleton instance.
    /// \return The singleton instance.
    ///
    static GrannyLibrary& instance();

    ///
    /// \brief Loads "granny2_x64.dll" and resolves all required functions.
    /// Repeated calls are no-ops returning the previous result.
    /// \return True on success, false if the dll could not be loaded or a required
    /// function was missing.
    ///
    bool Initialize();

    ///
    /// \brief Returns whether the library has been initialized successfully.
    /// \return True if initialized.
    ///
    [[nodiscard]] bool is_initialized() const;

    // File functions.

    ///
    /// \brief Reads an entire granny file.
    /// \param file_name Path of the granny file.
    /// \return The loaded file or nullptr on failure / not initialized.
    ///
    [[nodiscard]] GrannyFile* ReadEntireFile(const std::string& file_name) const;

    ///
    /// \brief Returns the file info of a loaded granny file.
    /// \param file Loaded granny file.
    /// \return The file info or nullptr on failure / not initialized.
    ///
    [[nodiscard]] GrannyFileInfo* GetFileInfo(GrannyFile* file) const;

    ///
    /// \brief Frees a previously loaded granny file.
    /// \param file Loaded granny file to free.
    ///
    void FreeFile(GrannyFile const* file) const;

    // Mesh functions.

    ///
    /// \brief Returns the total size in bytes of a vertex type definition array.
    /// \param type_definition Vertex type definitions describing the layout.
    /// \return Size in bytes, 0 on failure / not initialized.
    ///
    [[nodiscard]] int GetTotalTypeSize(GrannyDataTypeDefinition* type_definition) const;

    ///
    /// \brief Returns the vertex count of a mesh.
    /// \param mesh Mesh to query.
    /// \return Vertex count, 0 on failure / not initialized.
    ///
    [[nodiscard]] int GetMeshVertexCount(GrannyMesh const* mesh) const;

    ///
    /// \brief Copies the vertices of a mesh into the destination buffer.
    /// \param mesh Mesh to copy from.
    /// \param vertex_type Vertex type definitions describing the layout.
    /// \param dest_vertices Destination buffer.
    ///
    void CopyMeshVertices(
        GrannyMesh const* mesh, GrannyDataTypeDefinition const* vertex_type, void* dest_vertices
    ) const;

    ///
    /// \brief Returns the index count of a mesh.
    /// \param mesh Mesh to query.
    /// \return Index count, 0 on failure / not initialized.
    ///
    [[nodiscard]] int GetMeshIndexCount(GrannyMesh const* mesh) const;

    ///
    /// \brief Copies the indices of a mesh into the destination buffer.
    /// \param mesh Mesh to copy from.
    /// \param bytes_per_index Size of one index in bytes.
    /// \param dest_indices Destination buffer.
    ///
    void CopyMeshIndices(GrannyMesh const* mesh, int bytes_per_index, void* dest_indices) const;

    ///
    /// \brief Builds the composite 4x4 transform of a granny transform.
    /// \param transform Granny transform to build from.
    /// \param composite_4x4 Destination buffer (16 floats).
    ///
    void BuildCompositeTransform4x4(GrannyTransform const* transform, float* composite_4x4) const;

    ///
    /// \brief Returns whether a mesh is rigid.
    /// \param mesh Mesh to query.
    /// \return True if rigid, false otherwise / not initialized.
    ///
    [[nodiscard]] bool MeshIsRigid(GrannyMesh const* mesh) const;

    // Curve functions.

    ///
    /// \brief Converts a curve into the DAK32fC32f format.
    /// \param src_curve Source curve to convert.
    /// \param identity_vector Identity vector of the curve.
    /// \return The converted curve or nullptr on failure / not initialized.
    ///
    [[nodiscard]] GrannyCurve2* CurveConvertToDaK32fC32f(
        GrannyCurve2 const* src_curve, float const* identity_vector
    ) const;

    ///
    /// \brief Frees a previously converted curve.
    /// \param curve Curve to free.
    ///
    void FreeCurve(GrannyCurve2* curve) const;

    ///
    /// \brief Returns the dimension of a curve.
    /// \param curve Curve to query.
    /// \return Dimension, 0 on failure / not initialized.
    ///
    [[nodiscard]] int CurveGetDimension(GrannyCurve2 const* curve) const;

    ///
    /// \brief Evaluates a curve at time t.
    /// \param dimension Dimension of the curve.
    /// \param normalize Whether to normalize the result.
    /// \param backwards_loop Whether to loop backwards.
    /// \param curve Curve to evaluate.
    /// \param forwards_loop Whether to loop forwards.
    /// \param curve_duration Duration of the curve.
    /// \param t Time to evaluate at.
    /// \param result Destination buffer for the evaluated value.
    /// \param identity_vector Identity vector of the curve.
    ///
    void EvaluateCurveAtT(
        int dimension,
        bool normalize,
        bool backwards_loop,
        GrannyCurve2 const* curve,
        bool forwards_loop,
        float curve_duration,
        float t,
        float* result,
        float const* identity_vector
    ) const;

    ///
    /// \brief Returns the identity position vector of curves.
    /// \return Identity position vector or nullptr if not initialized.
    ///
    [[nodiscard]] float const* CurveIdentityPosition() const;

    ///
    /// \brief Returns the identity orientation vector of curves.
    /// \return Identity orientation vector or nullptr if not initialized.
    ///
    [[nodiscard]] float const* CurveIdentityOrientation() const;

    ///
    /// \brief Returns the identity scale shear vector of curves.
    /// \return Identity scale shear vector or nullptr if not initialized.
    ///
    [[nodiscard]] float const* CurveIdentityScaleShear() const;

    // Texture functions.

    ///
    /// \brief Returns whether a texture has an alpha channel.
    /// \param texture Texture to query.
    /// \return True if the texture has alpha, false otherwise / not initialized.
    ///
    [[nodiscard]] bool TextureHasAlpha(GrannyTexture const* texture) const;

    ///
    /// \brief Returns the RGBA8888 pixel layout of granny textures.
    /// \return Pixel layout or nullptr if not initialized.
    ///
    [[nodiscard]] GrannyPixelLayout const* Rgba8888PixelFormat() const;

    ///
    /// \brief Returns the RGB888 pixel layout of granny textures.
    /// \return Pixel layout or nullptr if not initialized.
    ///
    [[nodiscard]] GrannyPixelLayout const* Rgb888PixelFormat() const;

    ///
    /// \brief Copies a texture image into the destination buffer.
    /// \param texture Texture to copy from.
    /// \param image_index Index of the image.
    /// \param mip_index Index of the mipmap level.
    /// \param layout Pixel layout describing the destination format.
    /// \param dest_width Width of the destination buffer.
    /// \param dest_height Height of the destination buffer.
    /// \param dest_stride Stride of the destination buffer in bytes.
    /// \param pixels Destination buffer.
    ///
    void CopyTextureImage(
        GrannyTexture const* texture,
        int image_index,
        int mip_index,
        GrannyPixelLayout const* layout,
        int dest_width,
        int dest_height,
        int dest_stride,
        void* pixels
    ) const;

private:
    ///
    /// \brief Logs a fatal message for a function called before initialization.
    /// \param function_name Name of the granny function that was called.
    ///
    static void LogNotInitialized(const char* function_name);

    GrannyLibrary() = default;
    ~GrannyLibrary() = default;
    GrannyLibrary(const GrannyLibrary&) = delete;
    GrannyLibrary& operator=(const GrannyLibrary&) = delete;

    GrannyReadEntireFile_t read_entire_file_ = nullptr;
    GrannyGetFileInfo_t get_file_info_ = nullptr;
    GrannyFreeFile_t free_file_ = nullptr;
    GrannyDataTypeDefinition* pwnt3432_vertex_type_ = nullptr;
    GrannyGetTotalTypeSize_t get_total_type_size_ = nullptr;
    GrannyGetMeshVertexCount_t get_mesh_vertex_count_ = nullptr;
    GrannyCopyMeshVertices_t copy_mesh_vertices_ = nullptr;
    GrannyGetMeshIndexCount_t get_mesh_index_count_ = nullptr;
    GrannyCopyMeshIndices_t copy_mesh_indices_ = nullptr;
    GrannyBuildCompositeTransform4x4_t build_composite_transform4x4_ = nullptr;
    GrannyMeshIsRigid_t mesh_is_rigid_ = nullptr;
    GrannyComputeBasisConversion_t compute_basis_conversion_ = nullptr;
    GrannyTransformFile_t transform_file_ = nullptr;
    GrannyCurveMakeStaticDaK32fC32f_t curve_make_static_da_k32f_c32f_ = nullptr;
    GrannyCurveConvertToDaK32fC32f_t curve_convert_to_da_k32f_c32f_ = nullptr;
    GrannyFreeCurve_t free_curve_ = nullptr;
    GrannyCurveGetKnotCount_t curve_get_knot_count_ = nullptr;
    GrannyCurveGetDimension_t curve_get_dimension_ = nullptr;
    GrannyCurveGetDegree_t curve_get_degree_ = nullptr;
    float* curve_identity_position_ = nullptr;
    float* curve_identity_orientation_ = nullptr;
    float* curve_identity_scale_shear_ = nullptr;
    float* curve_identity_scale_ = nullptr;
    GrannyEvaluateCurveAtT_t evaluate_curve_at_t_ = nullptr;
    GrannyEvaluateCurveAtKnotIndex_t evaluate_curve_at_knot_index_ = nullptr;
    GrannyFindKnot_t find_knot_ = nullptr;
    GrannyFindCloseKnot_t find_close_knot_ = nullptr;
    GrannyCurveIsKeyframed_t curve_is_keyframed_ = nullptr;
    GrannyCurveInitializeFormat_t curve_initialize_format_ = nullptr;
    GrannyDataTypeDefinition* curve_data_da_identity_type_ = nullptr;
    GrannyTextureHasAlpha_t texture_has_alpha_ = nullptr;
    GrannyPixelLayout* rgba8888_pixel_format_ = nullptr;
    GrannyPixelLayout* rgb888_pixel_format_ = nullptr;
    GrannyCopyTextureImage_t copy_texture_image_ = nullptr;

    HMODULE dll_handle_ = nullptr;
    bool initialized_ = false;
};

}  // namespace GCL::Importer
