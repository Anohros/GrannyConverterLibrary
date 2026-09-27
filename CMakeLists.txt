cmake_minimum_required(VERSION 3.14)

project(GrannyConverterLibrary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GrannyConverterLibrarySources
    src/gcl/grannyconverterlibrary.cpp

    # Bindings
    src/gcl/bindings/abstractcurvekey.cpp
    src/gcl/bindings/animation.cpp
    src/gcl/bindings/bone.cpp
    src/gcl/bindings/bonebinding.cpp
    src/gcl/bindings/curvepositionkey.cpp
    src/gcl/bindings/curverotationkey.cpp
    src/gcl/bindings/curvescalekey.cpp
    src/gcl/bindings/material.cpp
    src/gcl/bindings/mesh.cpp
    src/gcl/bindings/model.cpp
    src/gcl/bindings/scene.cpp
    src/gcl/bindings/skeleton.cpp
    src/gcl/bindings/track.cpp

    # Exporter
    src/gcl/exporter/fbxexporter.cpp
    src/gcl/exporter/fbxexporteranimation.cpp
    src/gcl/exporter/fbxexportermaterial.cpp
    src/gcl/exporter/fbxexportermesh.cpp
    src/gcl/exporter/fbxexportermodule.cpp
    src/gcl/exporter/fbxexportermodulefactory.cpp
    src/gcl/exporter/fbxexporterskeleton.cpp

    # Importer
    src/gcl/importer/deboor.cpp
    src/gcl/importer/grannyformat.cpp
    src/gcl/importer/grannyimporter.cpp
    src/gcl/importer/grannyimporteranimation_deboor.cpp
    src/gcl/importer/grannyimporteranimation.cpp
    src/gcl/importer/grannyimportermaterial.cpp
    src/gcl/importer/grannyimportermodel.cpp
    src/gcl/importer/grannyimporterskeleton.cpp

    # Utilities
    src/gcl/utilities/datetime.cpp
    src/gcl/utilities/devilimageutility.cpp
    src/gcl/utilities/fbxsdkcommon.cpp
    src/gcl/utilities/filestreamutility.cpp
    src/gcl/utilities/logging.cpp
    src/gcl/utilities/materialutility.cpp
    src/gcl/utilities/stringutility.cpp
    src/gcl/utilities/textureutility.cpp
)

add_library(GrannyConverterLibrary STATIC ${GrannyConverterLibrarySources})

target_include_directories(GrannyConverterLibrary PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(GrannyConverterLibrary PRIVATE advapi32 shell32 user32 Kernel32 Ole32)
target_compile_definitions(GrannyConverterLibrary PRIVATE GRANNYCONVERTERLIBRARY_LIBRARY)

if(MSVC)
    target_compile_options(GrannyConverterLibrary PRIVATE /W4)
endif()

# DevIL SDK and FBX SDK
include("${PROJECT_SOURCE_DIR}/cmake/devilsdk.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/fbxsdk.cmake")

# The granny2 dll is loaded at runtime by the importer. Warn if it is missing so
# that a failed conversion is not only discovered at runtime.
set(GRANNY_DLL "${CMAKE_SOURCE_DIR}/external/granny2/granny2_x64.dll")
if(NOT EXISTS ${GRANNY_DLL})
    message(WARNING
        "Granny 3D DLL not found at ${GRANNY_DLL}. "
        "The library will still build, but importing granny files will fail at runtime. "
        "Place granny2_x64.dll (v2.9-2.12) into external/granny2/.")
endif()

# Add examples.
add_subdirectory(examples/converter)
