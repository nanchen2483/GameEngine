# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE sandbox_source_files "src/*.cpp")
file(GLOB_RECURSE sandbox_header_files "src/*.h")

# Add source to this project's executable.
add_executable (Sandbox
  ${sandbox_source_files}
  ${sandbox_header_files}
)

target_include_directories(Sandbox PRIVATE "src")

target_link_libraries(Sandbox PRIVATE Engine)

target_compile_definitions(Sandbox PRIVATE
    "_CRT_SECURE_NO_WARNINGS"
    "ENGINE_PLATFORM_WINDOWS"
)

add_custom_target(copy_sandbox_assets ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/asserts
					${CMAKE_CURRENT_BINARY_DIR}/asserts
					COMMENT "Copying assets into binary directory")

add_custom_target(copy_sandbox_resources ALL
					COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_CURRENT_SOURCE_DIR}/resources
					${CMAKE_CURRENT_BINARY_DIR}/resources
					COMMENT "Copying resources into binary directory")

add_dependencies(Sandbox copy_sandbox_assets copy_sandbox_resources)

add_custom_command(TARGET Sandbox PRE_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
					${CMAKE_CURRENT_BINARY_DIR}/imgui.ini
					COMMENT "Copying imgui.ini into binary directory")

set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")
