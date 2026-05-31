#include "gcl/importer/grannyformat.h"

#include "gcl/utilities/logging.h"

using namespace GCL::Utilities::Logging;

template <typename T>
T GetGrannyFunction(HMODULE hModule, const char* lpProcName) {
    auto grannyFunction = reinterpret_cast<T>(GetProcAddress(hModule, lpProcName));
    if (!grannyFunction) {
        char errorMessage[255];
        snprintf(
            errorMessage,
            sizeof(errorMessage),
            "Granny function \"%s\" not available in \"granny2_x64.dll\" library.",
            lpProcName
        );
        throw std::runtime_error(errorMessage);
    }
    return grannyFunction;
}

bool InitializeGrannyLibrary() {
    if (!std::ifstream("granny2_x64.dll").is_open()) {
        fatal("Could not locate \"granny2_x64.dll\" library.");
        return false;
    }

    auto grannyDllHandle = LoadLibraryW(L"granny2_x64.dll");
    if (!grannyDllHandle) {
        fatal("Could not load \"granny2_x64.dll\" library.");
        return false;
    }

    try {
        GrannyReadEntireFile =
            GetGrannyFunction<GrannyReadEntireFile_t>(grannyDllHandle, "GrannyReadEntireFile");
        GrannyGetFileInfo =
            GetGrannyFunction<GrannyGetFileInfo_t>(grannyDllHandle, "GrannyGetFileInfo");
        GrannyFreeFile = GetGrannyFunction<GrannyFreeFile_t>(grannyDllHandle, "GrannyFreeFile");
        GrannyGetTotalTypeSize =
            GetGrannyFunction<GrannyGetTotalTypeSize_t>(grannyDllHandle, "GrannyGetTotalTypeSize");
        GrannyPWNT3432VertexType = reinterpret_cast<GrannyDataTypeDefinition*>(
            *GetGrannyFunction<int*>(grannyDllHandle, "GrannyPWNT3432VertexType")
        );
        GrannyGetMeshVertexCount = GetGrannyFunction<GrannyGetMeshVertexCount_t>(
            grannyDllHandle, "GrannyGetMeshVertexCount"
        );
        GrannyGetMeshIndexCount = GetGrannyFunction<GrannyGetMeshIndexCount_t>(
            grannyDllHandle, "GrannyGetMeshIndexCount"
        );
        GrannyCopyMeshVertices =
            GetGrannyFunction<GrannyCopyMeshVertices_t>(grannyDllHandle, "GrannyCopyMeshVertices");
        GrannyCopyMeshIndices =
            GetGrannyFunction<GrannyCopyMeshIndices_t>(grannyDllHandle, "GrannyCopyMeshIndices");
        GrannyBuildCompositeTransform4x4 = GetGrannyFunction<GrannyBuildCompositeTransform4x4_t>(
            grannyDllHandle, "GrannyBuildCompositeTransform4x4"
        );
        GrannyMeshIsRigid =
            GetGrannyFunction<GrannyMeshIsRigid_t>(grannyDllHandle, "GrannyMeshIsRigid");
        GrannyComputeBasisConversion = GetGrannyFunction<GrannyComputeBasisConversion_t>(
            grannyDllHandle, "GrannyComputeBasisConversion"
        );
        GrannyTransformFile =
            GetGrannyFunction<GrannyTransformFile_t>(grannyDllHandle, "GrannyTransformFile");
        GrannyCurveMakeStaticDaK32fC32f = GetGrannyFunction<GrannyCurveMakeStaticDaK32fC32f_t>(
            grannyDllHandle, "GrannyCurveMakeStaticDaK32fC32f"
        );
        GrannyCurveConvertToDaK32fC32f = GetGrannyFunction<GrannyCurveConvertToDaK32fC32f_t>(
            grannyDllHandle, "GrannyCurveConvertToDaK32fC32f"
        );
        GrannyFreeCurve = GetGrannyFunction<GrannyFreeCurve_t>(grannyDllHandle, "GrannyFreeCurve");
        GrannyCurveGetKnotCount = GetGrannyFunction<GrannyCurveGetKnotCount_t>(
            grannyDllHandle, "GrannyCurveGetKnotCount"
        );
        GrannyCurveGetDimension = GetGrannyFunction<GrannyCurveGetDimension_t>(
            grannyDllHandle, "GrannyCurveGetDimension"
        );
        GrannyCurveGetDegree =
            GetGrannyFunction<GrannyCurveGetDegree_t>(grannyDllHandle, "GrannyCurveGetDegree");
        GrannyCurveIdentityPosition = GetGrannyFunction<GrannyCurveIdentityPosition_t>(
            grannyDllHandle, "GrannyCurveIdentityPosition"
        );
        GrannyCurveIdentityOrientation = GetGrannyFunction<GrannyCurveIdentityOrientation_t>(
            grannyDllHandle, "GrannyCurveIdentityOrientation"
        );
        GrannyCurveIdentityScaleShear = GetGrannyFunction<GrannyCurveIdentityScaleShear_t>(
            grannyDllHandle, "GrannyCurveIdentityScaleShear"
        );
        GrannyCurveIdentityScale = GetGrannyFunction<GrannyCurveIdentityScale_t>(
            grannyDllHandle, "GrannyCurveIdentityScale"
        );
        GrannyEvaluateCurveAtT =
            GetGrannyFunction<GrannyEvaluateCurveAtT_t>(grannyDllHandle, "GrannyEvaluateCurveAtT");
        GrannyEvaluateCurveAtKnotIndex = GetGrannyFunction<GrannyEvaluateCurveAtKnotIndex_t>(
            grannyDllHandle, "GrannyEvaluateCurveAtKnotIndex"
        );
        GrannyFindKnot = GetGrannyFunction<GrannyFindKnot_t>(grannyDllHandle, "GrannyFindKnot");
        GrannyFindCloseKnot =
            GetGrannyFunction<GrannyFindCloseKnot_t>(grannyDllHandle, "GrannyFindCloseKnot");
        GrannyCurveIsKeyframed =
            GetGrannyFunction<GrannyCurveIsKeyframed_t>(grannyDllHandle, "GrannyCurveIsKeyframed");
        GrannyCurveInitializeFormat = GetGrannyFunction<GrannyCurveInitializeFormat_t>(
            grannyDllHandle, "GrannyCurveInitializeFormat"
        );
        GrannyCurveDataDaIdentityType = GetGrannyFunction<GrannyCurveDataDaIdentityType_t>(
            grannyDllHandle, "GrannyCurveDataDaIdentityType"
        );
        GrannyTextureHasAlpha =
            GetGrannyFunction<GrannyTextureHasAlpha_t>(grannyDllHandle, "GrannyTextureHasAlpha");
        GrannyRGBA8888PixelFormat = *GetGrannyFunction<GrannyRGBA8888PixelFormat_t*>(
            grannyDllHandle, "GrannyRGBA8888PixelFormat"
        );
        GrannyRGB888PixelFormat = *GetGrannyFunction<GrannyRGB888PixelFormat_t*>(
            grannyDllHandle, "GrannyRGB888PixelFormat"
        );
        GrannyCopyTextureImage =
            GetGrannyFunction<GrannyCopyTextureImage_t>(grannyDllHandle, "GrannyCopyTextureImage");
    } catch (std::runtime_error& error) {
        fatal(error.what());
        return false;
    }

    return true;
}
