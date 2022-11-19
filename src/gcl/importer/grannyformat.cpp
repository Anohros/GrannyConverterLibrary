#include "gcl/importer/grannyformat.h"

#include "gcl/utilities/logging.h"

using namespace GCL::Utilities::Logging;

template <typename T>
T GetGrannyFunction(HMODULE hModule, const char* lpProcName)
{
    auto grannyFunction = reinterpret_cast<T>(GetProcAddress(hModule, lpProcName));
    if (!grannyFunction) {
        char errorMessage[255];
        snprintf(
            errorMessage,
            sizeof(errorMessage),
            "Granny function \"%s\" not available in \"granny2.dll\" library.",
            lpProcName);
        throw std::runtime_error(errorMessage);
    }
    return grannyFunction;
}

bool InitializeGrannyLibrary()
{
    if (!ifstream("granny2.dll").is_open()) {
        fatal("Could not locate \"granny2.dll\" library.");
        return false;
    }

    auto grannyDllHandle = LoadLibraryW(L"granny2.dll");
    if (!grannyDllHandle) {
        fatal("Could not load \"granny2.dll\" library.");
        return false;
    }

    try {
        GrannyReadEntireFile = GetGrannyFunction<GrannyReadEntireFile_t>(grannyDllHandle, "_GrannyReadEntireFile@4");
        GrannyGetFileInfo = GetGrannyFunction<GrannyGetFileInfo_t>(grannyDllHandle, "_GrannyGetFileInfo@4");
        GrannyFreeFile = GetGrannyFunction<GrannyFreeFile_t>(grannyDllHandle, "_GrannyFreeFile@4");
        GrannyPWNT3432VertexType = static_cast<int>(*GetGrannyFunction<int*>(grannyDllHandle, "GrannyPWNT3432VertexType"));
        GrannyGetMeshVertexCount = GetGrannyFunction<GrannyGetMeshVertexCount_t>(grannyDllHandle, "_GrannyGetMeshVertexCount@4");
        GrannyGetMeshIndexCount = GetGrannyFunction<GrannyGetMeshIndexCount_t>(grannyDllHandle, "_GrannyGetMeshIndexCount@4");
        GrannyCopyMeshVertices = GetGrannyFunction<GrannyCopyMeshVertices_t>(grannyDllHandle, "_GrannyCopyMeshVertices@12");
        GrannyCopyMeshIndices = GetGrannyFunction<GrannyCopyMeshIndices_t>(grannyDllHandle, "_GrannyCopyMeshIndices@12");
        GrannyBuildCompositeTransform4x4 = GetGrannyFunction<GrannyBuildCompositeTransform4x4_t>(grannyDllHandle, "_GrannyBuildCompositeTransform4x4@8");
        GrannyMeshIsRigid = GetGrannyFunction<GrannyMeshIsRigid_t>(grannyDllHandle, "_GrannyMeshIsRigid@4");
        GrannyComputeBasisConversion = GetGrannyFunction<GrannyComputeBasisConversion_t>(grannyDllHandle, "_GrannyComputeBasisConversion@36");
        GrannyTransformFile = GetGrannyFunction<GrannyTransformFile_t>(grannyDllHandle, "_GrannyTransformFile@28");
        GrannyCurveMakeStaticDaK32fC32f = GetGrannyFunction<GrannyCurveMakeStaticDaK32fC32f_t>(grannyDllHandle, "_GrannyCurveMakeStaticDaK32fC32f@28");
        GrannyCurveConvertToDaK32fC32f = GetGrannyFunction<GrannyCurveConvertToDaK32fC32f_t>(grannyDllHandle, "_GrannyCurveConvertToDaK32fC32f@8");
        GrannyFreeCurve = GetGrannyFunction<GrannyFreeCurve_t>(grannyDllHandle, "_GrannyFreeCurve@4");
        GrannyCurveGetKnotCount = GetGrannyFunction<GrannyCurveGetKnotCount_t>(grannyDllHandle, "_GrannyCurveGetKnotCount@4");
        GrannyCurveGetDimension = GetGrannyFunction<GrannyCurveGetDimension_t>(grannyDllHandle, "_GrannyCurveGetDimension@4");
        GrannyCurveGetDegree = GetGrannyFunction<GrannyCurveGetDegree_t>(grannyDllHandle, "_GrannyCurveGetDegree@4");
        GrannyCurveIdentityPosition = GetGrannyFunction<GrannyCurveIdentityPosition_t>(grannyDllHandle, "GrannyCurveIdentityPosition");
        GrannyCurveIdentityOrientation = GetGrannyFunction<GrannyCurveIdentityOrientation_t>(grannyDllHandle, "GrannyCurveIdentityOrientation");
        GrannyCurveIdentityScaleShear = GetGrannyFunction<GrannyCurveIdentityScaleShear_t>(grannyDllHandle, "GrannyCurveIdentityScaleShear");
        GrannyCurveIdentityScale = GetGrannyFunction<GrannyCurveIdentityScale_t>(grannyDllHandle, "GrannyCurveIdentityScale");
        GrannyEvaluateCurveAtT = GetGrannyFunction<GrannyEvaluateCurveAtT_t>(grannyDllHandle, "_GrannyEvaluateCurveAtT@36");
        GrannyEvaluateCurveAtKnotIndex = GetGrannyFunction<GrannyEvaluateCurveAtKnotIndex_t>(grannyDllHandle, "_GrannyEvaluateCurveAtKnotIndex@40");
        GrannyFindKnot = GetGrannyFunction<GrannyFindKnot_t>(grannyDllHandle, "_GrannyFindKnot@12");
        GrannyFindCloseKnot = GetGrannyFunction<GrannyFindCloseKnot_t>(grannyDllHandle, "_GrannyFindCloseKnot@16");
        GrannyCurveIsKeyframed = GetGrannyFunction<GrannyCurveIsKeyframed_t>(grannyDllHandle, "_GrannyCurveIsKeyframed@4");
        GrannyCurveInitializeFormat = GetGrannyFunction<GrannyCurveInitializeFormat_t>(grannyDllHandle, "_GrannyCurveInitializeFormat@4");
        GrannyCurveDataDaIdentityType = GetGrannyFunction<GrannyCurveDataDaIdentityType_t>(grannyDllHandle, "GrannyCurveDataDaIdentityType");
        GrannyTextureHasAlpha = GetGrannyFunction<GrannyTextureHasAlpha_t>(grannyDllHandle, "_GrannyTextureHasAlpha@4");
        GrannyRGBA8888PixelFormat = *GetGrannyFunction<GrannyRGBA8888PixelFormat_t*>(grannyDllHandle, "GrannyRGBA8888PixelFormat");
        GrannyRGB888PixelFormat = *GetGrannyFunction<GrannyRGB888PixelFormat_t*>(grannyDllHandle, "GrannyRGB888PixelFormat");
        GrannyCopyTextureImage = GetGrannyFunction<GrannyCopyTextureImage_t>(grannyDllHandle, "_GrannyCopyTextureImage@32");
    } catch (std::runtime_error& error) {
        fatal(error.what());
        return false;
    }

    return true;
}
