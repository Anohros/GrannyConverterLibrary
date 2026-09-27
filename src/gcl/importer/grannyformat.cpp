#include "gcl/importer/grannyformat.h"

#include <filesystem>
#include <format>

#include "gcl/utilities/logging.h"

using namespace GCL::Utilities::Logging;

namespace GCL::Importer {

template <typename T>
T GetGrannyFunction(HMODULE h_module, const char* lp_proc_name) {
    auto granny_function = reinterpret_cast<T>(GetProcAddress(h_module, lp_proc_name));
    if (!granny_function) {
        std::string error_message = std::format(
            R"(Granny function "{}" not available in "granny2_x64.dll" library.)", lp_proc_name
        );
        throw std::runtime_error(error_message);
    }
    return granny_function;
}

GrannyLibrary& GrannyLibrary::instance() {
    static GrannyLibrary instance;
    return instance;
}

void GrannyLibrary::LogNotInitialized(const char* function_name) {
    fatal(
        "Granny function \"%s\" called before the granny library was initialized. "
        "Construct a GrannyConverterLibrary first.",
        function_name
    );
}

bool GrannyLibrary::Initialize() {
    if (initialized_) {
        return true;
    }

    if (!std::filesystem::exists("granny2_x64.dll")) {
        fatal("Could not locate \"granny2_x64.dll\" library.");
        return false;
    }

    HMODULE granny_dll_handle = LoadLibraryW(L"granny2_x64.dll");
    if (!granny_dll_handle) {
        fatal("Could not load \"granny2_x64.dll\" library.");
        return false;
    }

    try {
        read_entire_file_ =
            GetGrannyFunction<GrannyReadEntireFile_t>(granny_dll_handle, "GrannyReadEntireFile");
        get_file_info_ =
            GetGrannyFunction<GrannyGetFileInfo_t>(granny_dll_handle, "GrannyGetFileInfo");
        free_file_ = GetGrannyFunction<GrannyFreeFile_t>(granny_dll_handle, "GrannyFreeFile");
        get_total_type_size_ = GetGrannyFunction<GrannyGetTotalTypeSize_t>(
            granny_dll_handle, "GrannyGetTotalTypeSize"
        );
        pwnt3432_vertex_type_ = GetGrannyFunction<GrannyDataTypeDefinition*>(
            granny_dll_handle, "GrannyPWNT3432VertexType"
        );
        get_mesh_vertex_count_ = GetGrannyFunction<GrannyGetMeshVertexCount_t>(
            granny_dll_handle, "GrannyGetMeshVertexCount"
        );
        get_mesh_index_count_ = GetGrannyFunction<GrannyGetMeshIndexCount_t>(
            granny_dll_handle, "GrannyGetMeshIndexCount"
        );
        copy_mesh_vertices_ = GetGrannyFunction<GrannyCopyMeshVertices_t>(
            granny_dll_handle, "GrannyCopyMeshVertices"
        );
        copy_mesh_indices_ =
            GetGrannyFunction<GrannyCopyMeshIndices_t>(granny_dll_handle, "GrannyCopyMeshIndices");
        build_composite_transform4x4_ = GetGrannyFunction<GrannyBuildCompositeTransform4x4_t>(
            granny_dll_handle, "GrannyBuildCompositeTransform4x4"
        );
        mesh_is_rigid_ =
            GetGrannyFunction<GrannyMeshIsRigid_t>(granny_dll_handle, "GrannyMeshIsRigid");
        compute_basis_conversion_ = GetGrannyFunction<GrannyComputeBasisConversion_t>(
            granny_dll_handle, "GrannyComputeBasisConversion"
        );
        transform_file_ =
            GetGrannyFunction<GrannyTransformFile_t>(granny_dll_handle, "GrannyTransformFile");
        curve_make_static_da_k32f_c32f_ = GetGrannyFunction<GrannyCurveMakeStaticDaK32fC32f_t>(
            granny_dll_handle, "GrannyCurveMakeStaticDaK32fC32f"
        );
        curve_convert_to_da_k32f_c32f_ = GetGrannyFunction<GrannyCurveConvertToDaK32fC32f_t>(
            granny_dll_handle, "GrannyCurveConvertToDaK32fC32f"
        );
        free_curve_ = GetGrannyFunction<GrannyFreeCurve_t>(granny_dll_handle, "GrannyFreeCurve");
        curve_get_knot_count_ = GetGrannyFunction<GrannyCurveGetKnotCount_t>(
            granny_dll_handle, "GrannyCurveGetKnotCount"
        );
        curve_get_dimension_ = GetGrannyFunction<GrannyCurveGetDimension_t>(
            granny_dll_handle, "GrannyCurveGetDimension"
        );
        curve_get_degree_ =
            GetGrannyFunction<GrannyCurveGetDegree_t>(granny_dll_handle, "GrannyCurveGetDegree");
        curve_identity_position_ = GetGrannyFunction<GrannyCurveIdentityPosition_t>(
            granny_dll_handle, "GrannyCurveIdentityPosition"
        );
        curve_identity_orientation_ = GetGrannyFunction<GrannyCurveIdentityOrientation_t>(
            granny_dll_handle, "GrannyCurveIdentityOrientation"
        );
        curve_identity_scale_shear_ = GetGrannyFunction<GrannyCurveIdentityScaleShear_t>(
            granny_dll_handle, "GrannyCurveIdentityScaleShear"
        );
        curve_identity_scale_ = GetGrannyFunction<GrannyCurveIdentityScale_t>(
            granny_dll_handle, "GrannyCurveIdentityScale"
        );
        evaluate_curve_at_t_ = GetGrannyFunction<GrannyEvaluateCurveAtT_t>(
            granny_dll_handle, "GrannyEvaluateCurveAtT"
        );
        evaluate_curve_at_knot_index_ = GetGrannyFunction<GrannyEvaluateCurveAtKnotIndex_t>(
            granny_dll_handle, "GrannyEvaluateCurveAtKnotIndex"
        );
        find_knot_ = GetGrannyFunction<GrannyFindKnot_t>(granny_dll_handle, "GrannyFindKnot");
        find_close_knot_ =
            GetGrannyFunction<GrannyFindCloseKnot_t>(granny_dll_handle, "GrannyFindCloseKnot");
        curve_is_keyframed_ = GetGrannyFunction<GrannyCurveIsKeyframed_t>(
            granny_dll_handle, "GrannyCurveIsKeyframed"
        );
        curve_initialize_format_ = GetGrannyFunction<GrannyCurveInitializeFormat_t>(
            granny_dll_handle, "GrannyCurveInitializeFormat"
        );
        curve_data_da_identity_type_ = GetGrannyFunction<GrannyCurveDataDaIdentityType_t>(
            granny_dll_handle, "GrannyCurveDataDaIdentityType"
        );
        texture_has_alpha_ =
            GetGrannyFunction<GrannyTextureHasAlpha_t>(granny_dll_handle, "GrannyTextureHasAlpha");
        rgba8888_pixel_format_ = *GetGrannyFunction<GrannyRGBA8888PixelFormat_t*>(
            granny_dll_handle, "GrannyRGBA8888PixelFormat"
        );
        rgb888_pixel_format_ = *GetGrannyFunction<GrannyRGB888PixelFormat_t*>(
            granny_dll_handle, "GrannyRGB888PixelFormat"
        );
        copy_texture_image_ = GetGrannyFunction<GrannyCopyTextureImage_t>(
            granny_dll_handle, "GrannyCopyTextureImage"
        );

        dll_handle_ = granny_dll_handle;
        initialized_ = true;
    } catch (std::runtime_error& error) {
        fatal(error.what());
        return false;
    }

    return true;
}

bool GrannyLibrary::is_initialized() const {
    return initialized_;
}

GrannyFile* GrannyLibrary::ReadEntireFile(const std::string& file_name) const {
    if (!read_entire_file_) {
        LogNotInitialized("GrannyReadEntireFile");
        return nullptr;
    }
    return read_entire_file_(file_name.c_str());
}

GrannyFileInfo* GrannyLibrary::GetFileInfo(GrannyFile* file) const {
    if (!get_file_info_) {
        LogNotInitialized("GrannyGetFileInfo");
        return nullptr;
    }
    return get_file_info_(file);
}

void GrannyLibrary::FreeFile(GrannyFile const* file) const {
    if (!free_file_) {
        LogNotInitialized("GrannyFreeFile");
        return;
    }
    free_file_(file);
}

int GrannyLibrary::GetTotalTypeSize(GrannyDataTypeDefinition* type_definition) const {
    if (!get_total_type_size_) {
        LogNotInitialized("GrannyGetTotalTypeSize");
        return 0;
    }
    return get_total_type_size_(type_definition);
}

int GrannyLibrary::GetMeshVertexCount(GrannyMesh const* mesh) const {
    if (!get_mesh_vertex_count_) {
        LogNotInitialized("GrannyGetMeshVertexCount");
        return 0;
    }
    return get_mesh_vertex_count_(mesh);
}

void GrannyLibrary::CopyMeshVertices(
    GrannyMesh const* mesh, GrannyDataTypeDefinition const* vertex_type, void* dest_vertices
) const {
    if (!copy_mesh_vertices_) {
        LogNotInitialized("GrannyCopyMeshVertices");
        return;
    }
    copy_mesh_vertices_(mesh, vertex_type, dest_vertices);
}

int GrannyLibrary::GetMeshIndexCount(GrannyMesh const* mesh) const {
    if (!get_mesh_index_count_) {
        LogNotInitialized("GrannyGetMeshIndexCount");
        return 0;
    }
    return get_mesh_index_count_(mesh);
}

void GrannyLibrary::CopyMeshIndices(
    GrannyMesh const* mesh, int bytes_per_index, void* dest_indices
) const {
    if (!copy_mesh_indices_) {
        LogNotInitialized("GrannyCopyMeshIndices");
        return;
    }
    copy_mesh_indices_(mesh, bytes_per_index, dest_indices);
}

void GrannyLibrary::BuildCompositeTransform4x4(
    GrannyTransform const* transform, float* composite_4x4
) const {
    if (!build_composite_transform4x4_) {
        LogNotInitialized("GrannyBuildCompositeTransform4x4");
        return;
    }
    build_composite_transform4x4_(transform, composite_4x4);
}

bool GrannyLibrary::MeshIsRigid(GrannyMesh const* mesh) const {
    if (!mesh_is_rigid_) {
        LogNotInitialized("GrannyMeshIsRigid");
        return false;
    }
    return mesh_is_rigid_(mesh);
}

GrannyCurve2* GrannyLibrary::CurveConvertToDaK32fC32f(
    GrannyCurve2 const* src_curve, float const* identity_vector
) const {
    if (!curve_convert_to_da_k32f_c32f_) {
        LogNotInitialized("GrannyCurveConvertToDaK32fC32f");
        return nullptr;
    }
    return curve_convert_to_da_k32f_c32f_(src_curve, identity_vector);
}

void GrannyLibrary::FreeCurve(GrannyCurve2* curve) const {
    if (!free_curve_) {
        LogNotInitialized("GrannyFreeCurve");
        return;
    }
    free_curve_(curve);
}

int GrannyLibrary::CurveGetDimension(GrannyCurve2 const* curve) const {
    if (!curve_get_dimension_) {
        LogNotInitialized("GrannyCurveGetDimension");
        return 0;
    }
    return curve_get_dimension_(curve);
}

void GrannyLibrary::EvaluateCurveAtT(
    int dimension,
    bool normalize,
    bool backwards_loop,
    GrannyCurve2 const* curve,
    bool forwards_loop,
    float curve_duration,
    float time,
    float* result,
    float const* identity_vector
) const {
    if (!evaluate_curve_at_t_) {
        LogNotInitialized("GrannyEvaluateCurveAtT");
        return;
    }
    evaluate_curve_at_t_(
        dimension,
        normalize,
        backwards_loop,
        curve,
        forwards_loop,
        curve_duration,
        time,
        result,
        identity_vector
    );
}

float const* GrannyLibrary::CurveIdentityPosition() const {
    if (!curve_identity_position_) {
        LogNotInitialized("GrannyCurveIdentityPosition");
        return nullptr;
    }
    return curve_identity_position_;
}

float const* GrannyLibrary::CurveIdentityOrientation() const {
    if (!curve_identity_orientation_) {
        LogNotInitialized("GrannyCurveIdentityOrientation");
        return nullptr;
    }
    return curve_identity_orientation_;
}

float const* GrannyLibrary::CurveIdentityScaleShear() const {
    if (!curve_identity_scale_shear_) {
        LogNotInitialized("GrannyCurveIdentityScaleShear");
        return nullptr;
    }
    return curve_identity_scale_shear_;
}

bool GrannyLibrary::TextureHasAlpha(GrannyTexture const* texture) const {
    if (!texture_has_alpha_) {
        LogNotInitialized("GrannyTextureHasAlpha");
        return false;
    }
    return texture_has_alpha_(texture);
}

GrannyPixelLayout const* GrannyLibrary::Rgba8888PixelFormat() const {
    if (!rgba8888_pixel_format_) {
        LogNotInitialized("GrannyRGBA8888PixelFormat");
        return nullptr;
    }
    return rgba8888_pixel_format_;
}

GrannyPixelLayout const* GrannyLibrary::Rgb888PixelFormat() const {
    if (!rgb888_pixel_format_) {
        LogNotInitialized("GrannyRGB888PixelFormat");
        return nullptr;
    }
    return rgb888_pixel_format_;
}

void GrannyLibrary::CopyTextureImage(
    GrannyTexture const* texture,
    int image_index,
    int mip_index,
    GrannyPixelLayout const* layout,
    int dest_width,
    int dest_height,
    int dest_stride,
    void* pixels
) const {
    if (!copy_texture_image_) {
        LogNotInitialized("GrannyCopyTextureImage");
        return;
    }
    copy_texture_image_(
        texture, image_index, mip_index, layout, dest_width, dest_height, dest_stride, pixels
    );
}

}  // namespace GCL::Importer
