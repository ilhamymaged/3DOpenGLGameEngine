cmake_minimum_required (VERSION 3.10)
project ("3D Game Engine")

#Disable GLFW components we don't need
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

#Add GLFW 
add_subdirectory(thirdparty/glfw)

#Add IamGui
add_subdirectory(thirdparty/imgui)

#Collect source files
file(GLOB_RECURSE SRC_FILES src/*.cpp)
list(APPEND SRC_FILES src/glad.c)

#Create executable
#if (WIN32)
#    add_executable (CMakeTarget WIN32 ${SRC_FILES})
#else()
add_executable (CMakeTarget ${SRC_FILES})
#endif()

#Include directories
target_include_directories(CMakeTarget PRIVATE include)

#Link libraries
target_link_libraries(CMakeTarget PRIVATE glfw ImGui)

if (WIN32)
    target_link_libraries(CMakeTarget PRIVATE opengl32)
endif()