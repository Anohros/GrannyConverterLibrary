#pragma once

#include <Windows.h>
#include <fstream>

using namespace std;

// Granny specific data structures.

typedef float GrannyTriple[3];
typedef float GrannyQuad[4];
typedef float GrannyMatrix4x4[4][4];

///
/// \brief Stores data of pixel layout.
///
struct GrannyPixelLayout {
    int BytesPerPixel;
    int ShiftForComponent[4];
    int BitsForComponent[4];
};

///
/// \brief Stores of data of mip level of a texture image.
///
struct GrannyTextureMIPLevel {
    int Stride;
    int PixelByteCount;
    void* PixelBytes;
};

///
/// \brief Stores details for a texture image.
///
struct GrannyTextureImage {
    int MIPLevelCount;
    GrannyTextureMIPLevel* MIPLevels;
};

///
/// \brief Stores a variant of data structure.
///
struct GrannyVariant {
    void* Type;
    void* Object;
};

///
/// \brief Stores all data of a texture.
///
struct GrannyTexture {
    char const* FromFileName;
    int TextureType;
    int Width;
    int Height;
    int Encoding;
    int SubFormat;
    GrannyPixelLayout Layout;
    int ImageCount;
    GrannyTextureImage* Images;
    GrannyVariant ExtendedData;
};

struct GrannyMaterialMap;

///
/// \brief Stores all data of a material.
///
struct GrannyMaterial {
    char const* Name;
    int MapCount;
    GrannyMaterialMap* Maps;
    GrannyTexture* Texture;
    GrannyVariant ExtendedData;
};

///
/// \brief Stores a material map.
///
struct GrannyMaterialMap {
    char const* Usage;
    GrannyMaterial* Material;
};

///
/// \brief Stores a reference to a material.
///
struct GrannyMaterialBinding {
    GrannyMaterial* Material;
};

///
/// \brief Stores all transform data of a transformation.
///
struct GrannyTransform {
    unsigned int Flags;
    float Position[3];
    float Orientation[4];
    float ScaleShear[3][3];
};

///
/// \brief Stores a variant of animation curve.
///
struct GrannyCurve2 {
    GrannyVariant CurveData;
};

///
/// \brief Stores all data of a animation transform track.
///
struct GrannyTransformTrack {
    char const* Name;
    int Flags;
    GrannyCurve2 OrientationCurve;
    GrannyCurve2 PositionCurve;
    GrannyCurve2 ScaleShearCurve;
};

///
/// \brief Stores all data of a animation track group.
///
struct GrannyTrackGroup {
    char const* Name;
    void* VectorTrackCount;
    void* VectorTracks;
    int TransformTrackCount;
    GrannyTransformTrack* TransformTracks;
    void* TransformLODErrorCount;
    void* TransformLODErrors;
    void* TextTrackCount;
    void* TextTracks;
    GrannyTransform InitialPlacement;
    int Flags;
    void* LoopTranslation;
    void* PeriodicLoop;
    GrannyVariant ExtendedData;
};

///
/// \brief Stores all data of a bone binding.
///
struct GrannyBoneBinding {
    char const* BoneName;
    GrannyTriple OBBMin;
    GrannyTriple OBBMax;
    int TriangleCount;
    int* TriangleIndices;
};

///
/// \brief Stores all data of a bone.
///
struct GrannyBone {
    char const* Name;
    int ParentIndex;
    GrannyTransform LocalTransform;
    GrannyMatrix4x4 InverseWorld4x4;
    float LODError;
    GrannyVariant ExtendedData;
};

///
/// \brief Stores all data of a skeleton.
///
struct GrannySkeleton {
    char const* Name;
    int BoneCount;
    GrannyBone* Bones;
    int LODType;
    GrannyVariant ExtendedData;
};

///
/// \brief Stores all data of a triangle group.
///
struct GrannyTriMaterialGroup {
    int MaterialIndex;
    int TriFirst;
    int TriCount;
};

///
/// \brief Stores all data of a triangle.
///
struct GrannyTriTopology {
    int GroupCount;
    GrannyTriMaterialGroup* Groups;
    int IndexCount;
    int* Indices;
    int Index16Count;
    unsigned short* Indices16;
    int VertexToVertexCount;
    int* VertexToVertexMap;
    int VertexToTriangleCount;
    int* VertexToTriangleMap;
    int SideToNeighborCount;
    unsigned int* SideToNeighborMap;
    int PolygonIndexStartCount;
    int* PolygonIndexStarts;
    int PolygonIndexCount;
    int* PolygonIndices;
    int BonesForTriangleCount;
    int* BonesForTriangle;
    int TriangleToBoneCount;
    int* TriangleToBoneIndices;
    int TriAnnotationSetCount;
    void* TriAnnotationSets;
};

///
/// \brief Stores all vertex data of a mesh.
///
struct GrannyVertexData {
    void* VertexType;
    int VertexCount;
    unsigned char* Vertices;
    int VertexComponentNameCount;
    char const** VertexComponentNames;
    int VertexAnnotationSetCount;
    void* VertexAnnotationSets;
};

///
/// \brief Stores all data of a mesh.
///
struct GrannyMesh {
    char const* Name;
    GrannyVertexData* PrimaryVertexData;
    int MorphTargetCount;
    void* MorphTargets;
    GrannyTriTopology* PrimaryTopology;
    int MaterialBindingCount;
    GrannyMaterialBinding* MaterialBindings;
    int BoneBindingCount;
    GrannyBoneBinding* BoneBindings;
    GrannyVariant ExtendedData;
};

///
/// \brief Stores a reference to a mesh also called MeshBinding.
///
struct GrannyModelMeshBinding {
    GrannyMesh* Mesh;
};

///
/// \brief Stores all data of a model.
///
struct GrannyModel {
    char const* Name;
    GrannySkeleton* Skeleton;
    GrannyTransform InitialPlacement;
    int MeshBindingCount;
    GrannyModelMeshBinding* MeshBindings;
    GrannyVariant ExtendedData;
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
    float Position[3];
    unsigned char BoneWeights[4];
    unsigned char BoneIndices[4];
    float Normal[3];
    float UV[2];
};

///
/// \brief Stores header of an animation curve like curve format or degree elevation of a curve.
///
struct GrannyCurveDataHeader {
    unsigned char Format;
    unsigned char Degree;
};

///
/// \brief Stores animation curve data for variant of a keyframe based constant curve.
///
struct GrannyCurveDataDAK32fC32f {
    GrannyCurveDataHeader CurveDataHeader;
    short Padding;
    int KnotCount;
    float* Knots;
    int ControlCount;
    float* Controls;
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
/// \brief Stores all data of a animation like name and tracks.
///
struct GrannyAnimation {
    char const* Name;
    float Duration;
    float TimeStep;
    float Oversampling;
    int TrackGroupCount;
    GrannyTrackGroup** TrackGroups;
    int DefaultLoopCount;
    int Flags;
    GrannyVariant ExtendedData;
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
/// \brief Stores type definitions .e.g. custom types for extensions.
///
struct GrannyDataTypeDefinition {
    void* Type;
    char const* Name;
    GrannyDataTypeDefinition* ReferenceType;
    void* ArrayWidth;
    void* Extra[3];
    void* Ignored_Ignored;
};

///
/// \brief Stores all header and data blocks of a granny file.
///
struct GrannyFile {
    void* IsByteReversed;
    void* Header;
    void* SourceMagicValue;
    void* SectionCount;
    void** Sections;
    bool* Marshalled;
    bool* IsUserMemory;
    void* ConversionBuffer;
    unsigned int ConversionBufferSize;
};

///
/// \brief Stores all raw data of a granny file.
///
struct GrannyFileInfo {
    void* ArtToolInfo;
    void* ExporterInfo;
    char const* FromFileName;
    int TextureCount;
    GrannyTexture** Textures;
    int MaterialCount;
    GrannyMaterial** Materials;
    int SkeletonCount;
    GrannySkeleton** Skeletons;
    int VertexDataCount;
    void* VertexDatas;
    int TriTopologyCount;
    GrannyTriTopology** TriTopologies;
    int MeshCount;
    GrannyMesh** Meshes;
    int ModelCount;
    GrannyModel** Models;
    int TrackGroupCount;
    GrannyTrackGroup** TrackGroups;
    int AnimationCount;
    GrannyAnimation** Animations;
    GrannyVariant ExtendedData;
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

typedef GrannyFile*(__stdcall* GrannyReadEntireFile_t)(const char* FileName);
typedef GrannyFileInfo*(__stdcall* GrannyGetFileInfo_t)(GrannyFile* File);
typedef void(__stdcall* GrannyFreeFile_t)(GrannyFile* File);
typedef int(__stdcall* GrannyGetMeshVertexCount_t)(GrannyMesh const* Mesh);
typedef void(__stdcall* GrannyCopyMeshVertices_t)(GrannyMesh const* Mesh, int VertexType, void* DestVertices);
typedef int(__stdcall* GrannyGetMeshIndexCount_t)(GrannyMesh const* Mesh);
typedef void(__stdcall* GrannyCopyMeshIndices_t)(GrannyMesh const* Mesh, int BytesPerIndex, void* DestIndices);
typedef void(__stdcall* GrannyBuildCompositeTransform4x4_t)(GrannyTransform const* Transform, float* Composite4x4);
typedef bool(__stdcall* GrannyMeshIsRigid_t)(GrannyMesh const* Mesh);

typedef bool(__stdcall* GrannyComputeBasisConversion_t)(
    GrannyFileInfo const* FileInfo,
    float DesiredUnitsPerMeter,
    float const* DesiredOrigin3,
    float const* DesiredRight3,
    float const* DesiredUp3,
    float const* DesiredBack3,
    float* ResultAffine3,
    float* ResultLinear3x3,
    float* ResultInverseLinear3x);

typedef void(__stdcall* GrannyTransformFile_t)(
    GrannyFileInfo* FileInfo,
    float const* Affine3,
    float const* Linear3x3,
    float const* InverseLinear3x3,
    float AffineTolerance,
    float LinearTolerance,
    unsigned Flags);

typedef void(__stdcall* GrannyCurveMakeStaticDaK32fC32f_t)(
    GrannyCurve2* Curve,
    GrannyCurveDataDAK32fC32f* CurveData,
    int KnotCount,
    int Degree,
    int Dimension,
    float const* Knots,
    float const* Controls);

typedef GrannyCurve2*(__stdcall* GrannyCurveConvertToDaK32fC32f_t)(
    GrannyCurve2 const* SrcCurve,
    float const* IdentityVector);

typedef void(__stdcall* GrannyFreeCurve_t)(GrannyCurve2* Curve);
typedef int(__stdcall* GrannyCurveGetKnotCount_t)(GrannyCurve2 const* Curve);
typedef int(__stdcall* GrannyCurveGetDimension_t)(GrannyCurve2 const* Curve);
typedef int(__stdcall* GrannyCurveGetDegree_t)(GrannyCurve2 const* Curve);
typedef float* GrannyCurveIdentityPosition_t;
typedef float* GrannyCurveIdentityOrientation_t;
typedef float* GrannyCurveIdentityScaleShear_t;
typedef float* GrannyCurveIdentityScale_t;

typedef void(__stdcall* GrannyEvaluateCurveAtT_t)(
    int Dimension,
    bool Normalize,
    bool BackwardsLoop,
    GrannyCurve2 const* Curve,
    bool ForwardsLoop,
    float CurveDuration,
    float t,
    float* Result,
    float const* IdentityVector);

typedef void(__stdcall* GrannyEvaluateCurveAtKnotIndex_t)(
    int Dimension,
    bool Normalize,
    bool BackwardsLoop,
    GrannyCurve2 const* Curve,
    bool ForwardsLoop,
    float CurveDuration,
    int KnotIndex,
    float t,
    float* Result,
    float const* IdentityVector);

typedef int(__stdcall* GrannyFindKnot_t)(
    int KnotCount,
    float* Knots,
    float t);

typedef int(__stdcall* GrannyFindCloseKnot_t)(
    int KnotCount,
    float* Knots,
    float t,
    int StartinIndex);

typedef bool(__stdcall* GrannyCurveIsKeyframed_t)(
    GrannyCurve2 const* Curve);

typedef void(__stdcall* GrannyCurveInitializeFormat_t)(
    GrannyCurve2* Curve);

typedef GrannyDataTypeDefinition* GrannyCurveDataDaIdentityType_t;

// Declarations for required functions of granny2 dll.
// Declarations will get assigned by InitializeGrannyLibrary function.

inline GrannyReadEntireFile_t GrannyReadEntireFile = nullptr;
inline GrannyGetFileInfo_t GrannyGetFileInfo = nullptr;
inline GrannyFreeFile_t GrannyFreeFile = nullptr;
inline int GrannyPWNT3432VertexType = 0;
inline GrannyGetMeshVertexCount_t GrannyGetMeshVertexCount = nullptr;
inline GrannyGetMeshIndexCount_t GrannyGetMeshIndexCount = nullptr;
inline GrannyCopyMeshVertices_t GrannyCopyMeshVertices = nullptr;
inline GrannyCopyMeshIndices_t GrannyCopyMeshIndices = nullptr;
inline GrannyBuildCompositeTransform4x4_t GrannyBuildCompositeTransform4x4 = nullptr;
inline GrannyMeshIsRigid_t GrannyMeshIsRigid = nullptr;
inline GrannyComputeBasisConversion_t GrannyComputeBasisConversion = nullptr;
inline GrannyTransformFile_t GrannyTransformFile = nullptr;
inline GrannyCurveMakeStaticDaK32fC32f_t GrannyCurveMakeStaticDaK32fC32f = nullptr;
inline GrannyCurveConvertToDaK32fC32f_t GrannyCurveConvertToDaK32fC32f = nullptr;
inline GrannyFreeCurve_t GrannyFreeCurve = nullptr;
inline GrannyCurveGetKnotCount_t GrannyCurveGetKnotCount = nullptr;
inline GrannyCurveGetDimension_t GrannyCurveGetDimension = nullptr;
inline GrannyCurveGetDegree_t GrannyCurveGetDegree = nullptr;
inline GrannyCurveIdentityPosition_t GrannyCurveIdentityPosition = nullptr;
inline GrannyCurveIdentityOrientation_t GrannyCurveIdentityOrientation = nullptr;
inline GrannyCurveIdentityScaleShear_t GrannyCurveIdentityScaleShear = nullptr;
inline GrannyCurveIdentityScale_t GrannyCurveIdentityScale = nullptr;
inline GrannyEvaluateCurveAtT_t GrannyEvaluateCurveAtT = nullptr;
inline GrannyEvaluateCurveAtKnotIndex_t GrannyEvaluateCurveAtKnotIndex = nullptr;
inline GrannyFindKnot_t GrannyFindKnot = nullptr;
inline GrannyFindCloseKnot_t GrannyFindCloseKnot = nullptr;
inline GrannyCurveIsKeyframed_t GrannyCurveIsKeyframed = nullptr;
inline GrannyCurveInitializeFormat_t GrannyCurveInitializeFormat = nullptr;
inline GrannyCurveDataDaIdentityType_t GrannyCurveDataDaIdentityType = nullptr;

///
/// \brief Returns function pointer to a function from granny2 dll.
/// \param hModule Granny library
/// \param lpProcName Function name
/// \return
///
template <typename T>
T GetGrannyFunction(HMODULE hModule, const char* lpProcName);

///
/// \brief Initialize functions from granny2 dll.
///
/// Assigns required type defined functions from granny2 dll to declarations above
/// so that the declared functions above can be used during the import.
///
/// \return Success or failure
///
bool InitializeGrannyLibrary();
