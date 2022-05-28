# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE xeditor_source_files "src/*.cpp")
file(GLOB_RECURSE xeditor_header_files "src/*.h")

# Add source to this project's executable.
add_executable (xEditor
  ${xeditor_source_files}
  ${xeditor_header_files}
)

target_include_directories(xEditor PRIVATE "src")

target_link_libraries(xEditor PRIVATE Engine)

target_compile_definitions(xEditor PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "ENGINE_PLATFORM_WINDOWS"
)

add_custom_target(copy_xeditor_assets ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/asserts
					${CMAKE_CURRENT_BINARY_DIR}/asserts
					COMMENT "Copying assets into binary directory")

add_custom_target(copy_xeditor_resources ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
					${CMAKE_CURRENT_BINARY_DIR}/resources
					COMMENT "Copying resources into binary directory")

add_dependencies(xEditor copy_xeditor_assets copy_xeditor_resources)

add_custom_command(TARGET xEditor PRE_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
					${CMAKE_CURRENT_BINARY_DIR}/imgui.ini
					COMMENT "Copying imgui.ini into binary directory")

set_property(TARGET xEditor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/xEditor")
