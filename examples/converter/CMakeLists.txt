cmake_minimum_required(VERSION 3.14)

project(ConverterExample LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ConverterExampleSources
    "*.cpp"
    "*.h"
)

add_executable(ConverterExample
  ${ConverterExampleSources}
)

target_include_directories(ConverterExample PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(ConverterExample GrannyConverterLibrary)
