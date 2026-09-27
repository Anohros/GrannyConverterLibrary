cmake_minimum_required(VERSION 3.14)

project(ConverterExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ConverterExample main.cpp)

target_link_libraries(ConverterExample PRIVATE GrannyConverterLibrary)

# Copy all dlls.
add_custom_command(TARGET ConverterExample POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_RUNTIME_DLLS:ConverterExample> $<TARGET_FILE_DIR:ConverterExample>
  COMMAND_EXPAND_LISTS
)

# Expected location of the granny2 dll.
set(GRANNY_DLL "${CMAKE_SOURCE_DIR}/external/granny2/granny2_x64.dll")

# Copy granny2 dll.
if(EXISTS ${GRANNY_DLL})
  add_custom_command(TARGET ConverterExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRANNY_DLL} $<TARGET_FILE_DIR:ConverterExample>
  )
endif()
