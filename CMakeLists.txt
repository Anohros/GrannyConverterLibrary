cmake_minimum_required(VERSION 3.14)

project(GrannyConverterLibrary LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE GrannyConverterLibrarySources
    "src/*.cpp"
    "src/*.h"
)

add_library(GrannyConverterLibrary STATIC
  ${GrannyConverterLibrarySources}
)

target_include_directories(GrannyConverterLibrary PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(GrannyConverterLibrary PRIVATE advapi32 shell32 user32 Kernel32 Ole32)
target_compile_definitions(GrannyConverterLibrary PRIVATE GRANNYCONVERTERLIBRARY_LIBRARY)

# DevIL SDK and FBX SDK
include("${PROJECT_SOURCE_DIR}/cmake/devilsdk.cmake")
include("${PROJECT_SOURCE_DIR}/cmake/fbxsdk.cmake")
