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

target_link_libraries(ConverterExample GrannyConverterLibrary)

# Copy all dlls.
add_custom_command(TARGET ConverterExample POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_RUNTIME_DLLS:ConverterExample> $<TARGET_FILE_DIR:ConverterExample>
  COMMAND_EXPAND_LISTS
)

# Expected location of the granny2 dll.
set(GRANNY_DLL "${PROJECT_SOURCE_DIR}/../../external/granny2/granny2.dll")

# Copy granny2 dll.
if(EXISTS ${GRANNY_DLL})
  add_custom_command(TARGET ConverterExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRANNY_DLL} $<TARGET_FILE_DIR:ConverterExample>
  )
endif()
