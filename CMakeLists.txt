cmake_minimum_required (VERSION 3.1)
cmake_policy(VERSION 3.1)

set(PROJECT_NAME _vdynamics)
project(${PROJECT_NAME})

# set flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-Werror -Og")
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# find packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/third_party/cmake")
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
find_package(Freetype REQUIRED)
message(STATUS "Found ASSIMP in ${FREETYPE_INCLUDE_DIR}")

add_subdirectory(third_party/pybind11)

# set LIBS based on OS
if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype irrKlang)
elseif(UNIX AND NOT APPLE)
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
  INCLUDE_DIRECTORIES(/System/Library/Frameworks)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
  FIND_LIBRARY(IOKit_LIBRARY IOKit)
  FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
  MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
  SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  SET(APPLE_LIBS ${APPLE_LIBS} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
  set(LIBS ${LIBS} ${APPLE_LIBS})
else()
  set(LIBS )
endif(WIN32)

# add GLAD to LIBS
add_library(GLAD "third_party/glad/glad.c")
set_property(TARGET GLAD PROPERTY POSITION_INDEPENDENT_CODE ON)
set(LIBS ${LIBS} GLAD)

# add stb_image to LIBS
add_library(STB_IMAGE "third_party/stb_image/stb_image.cpp")
set_property(TARGET STB_IMAGE PROPERTY POSITION_INDEPENDENT_CODE ON)
add_library(STB_IMAGE_WRITE "third_party/stb_image/stb_image_write.cpp")
set_property(TARGET STB_IMAGE_WRITE PROPERTY POSITION_INDEPENDENT_CODE ON)
set(LIBS ${LIBS} STB_IMAGE STB_IMAGE_WRITE)

# include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# set source files
set(SOURCES ${SOURCES} 
    src/shader_s.cpp
    src/texture_s.cpp
    src/file_io.cpp
    src/camera.cpp
    src/vis3d.cpp
    src/geometry/sphere.cpp)

# set pybind11 binding files
set(BINDINGS 
    src/main_pyb.cpp) 

# create shared library 
add_library(${PROJECT_NAME} MODULE "${SOURCES}" "${BINDINGS}")
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBS} pybind11::module Eigen3::Eigen)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                     SUFFIX "${PYTHON_MODULE_EXTENSION}")
