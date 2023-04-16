add_library(DevILSDK::DevIL SHARED IMPORTED)

set_target_properties(DevILSDK::DevIL PROPERTIES
    IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/external/devilsdk/lib/x86/Release/DevIL.lib"
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/devilsdk/lib/x86/Release/DevIL.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/devilsdk/include"
)

add_library(DevILSDK::ILU SHARED IMPORTED)

set_target_properties(DevILSDK::ILU PROPERTIES
    IMPORTED_IMPLIB "${PROJECT_SOURCE_DIR}/external/devilsdk/lib/x86/Release/ILU.lib"
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/external/devilsdk/lib/x86/Release/ILU.dll"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/external/devilsdk/include"
)

target_link_libraries(GrannyConverterLibrary PUBLIC DevILSDK::DevIL DevILSDK::ILU)
