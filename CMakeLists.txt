# Copyright (C) 2018 Tomasz Gałaj

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(XentuEngine)


# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


# find packages
find_package(OpenGL)
find_package(GLM)
find_package(glew)
find_package(glfw3 REQUIRED)
find_package(lua)


# source files
set(XEN_SRC_FILES
	src/Application.cpp
	src/Helper.h
	src/StartupLua.h
	src/XentuGame.cpp
	src/XentuGame.cpp
	src/audio/AudioPlayer.cpp
	src/audio/AudioPlayer.h
	src/audio/Sound.cpp
	src/audio/Sound.h
	src/filesystem/AssetManager.cpp
	src/filesystem/AssetManager.h
	src/filesystem/Configuration.cpp
	src/filesystem/Configuration.h
	src/graphics/Batch.h
	src/graphics/Color.cpp
	src/graphics/Color.h
	src/graphics/Quad.cpp
	src/graphics/Quad.h
	src/graphics/Rect.cpp
	src/graphics/Rect.h
	src/graphics/Renderer2D.cpp
	src/graphics/Renderer2D.h
	src/graphics/Sprite.cpp
	src/graphics/Sprite.h
	src/graphics/SpriteMap.cpp
	src/graphics/SpriteMap.h
	src/graphics/Texture.cpp
	src/graphics/Texture.h
	src/graphics/Transform.cpp
	src/graphics/Transform.h
	src/graphics/Transformable.cpp
	src/graphics/Transformable.h
	src/graphics/Vector2.h
	src/graphics/Vertex.cpp
	src/graphics/Vertex.h
	src/graphics/Viewport.h
	src/graphics/Viewport.cpp
	src/input/InputManager.cpp
	src/input/InputManager.h
)


# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src"
	"${CMAKE_SOURCE_DIR}/src/libraries"
	"${CMAKE_SOURCE_DIR}/thirdparty"
)


# compile list of libs (that will be linked to the exe)
set(XEN_LIBS
	mingw32
	"${OPENGL_LIBRARIES}"
	"${GLEW_LIBRARY}"
	"${GLFW3_LIBRARY}"
	"${LUA_LIBRARY}"
)


# Define the executable
add_executable(${PROJECT_NAME} ${XEN_SRC_FILES})


# Define the link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${XEN_LIBS})


if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")