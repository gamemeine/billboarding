cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(Billboarding CXX)



#
# GLAD configuration
#
set(GLAD_OUT_DIR        "${CMAKE_CURRENT_BINARY_DIR}" CACHE STRING "Output directory" FORCE)
set(GLAD_PROFILE        "core" CACHE STRING "OpenGL profile" FORCE)
set(GLAD_API            "gl=4.0" CACHE STRING "API type/version pairs, like \"gl=3.2,gles=\", no version means latest" FORCE)
set(GLAD_GENERATOR      "c" CACHE STRING "Language to generate the binding for" FORCE)
set(GLAD_EXTENSIONS     "" CACHE STRING "Path to extensions file or comma separated list of extensions, if missing all extensions are included" FORCE)
set(GLAD_SPEC           "gl" CACHE STRING "Name of the spec" FORCE)
set(GLAD_ALL_EXTENSIONS OFF CACHE BOOL "Include all extensions instead of those specified by GLAD_EXTENSIONS" FORCE)
set(GLAD_NO_LOADER      OFF CACHE BOOL "No loader" FORCE)
set(GLAD_REPRODUCIBLE   OFF CACHE BOOL "Reproducible build" FORCE)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#
# Billboarding source files
#
include_directories(include/)
set(sources 
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/Window.cpp
    ${SOURCE_DIR}/Scene.cpp
    ${SOURCE_DIR}/Program.cpp
    ${SOURCE_DIR}/Texture.cpp
    ${SOURCE_DIR}/Mesh.cpp
    ${SOURCE_DIR}/Model.cpp
    ${SOURCE_DIR}/Camera.cpp
    ${SOURCE_DIR}/Billboard.cpp
)

add_executable(billboarding ${sources} ${includes})

#
# Dependencies
#
include(${CMAKE_DIR}/LinkGLFW.cmake)
LinkGLFW(billboarding PRIVATE)

include(${CMAKE_DIR}/LinkGLAD.cmake)
LinkGLAD(billboarding PRIVATE)

include(${CMAKE_DIR}/LinkGLM.cmake)
LinkGLM(billboarding PRIVATE)

include(${CMAKE_DIR}/LinkASSIMP.cmake)
LinkASSIMP(billboarding PRIVATE)

find_package(OpenGL REQUIRED)
if (OpenGL_FOUND)
    target_include_directories(billboarding PRIVATE ${OPENGL_INCLDUE_DIRS})
    target_link_libraries(billboarding PRIVATE ${OPENGL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenGL was not found on the system")
endif()


set_target_properties(billboarding PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

set_target_properties(billboarding PROPERTIES FOLDER ${PROJECT_NAME})