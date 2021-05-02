# Copyright (C) 2018 Tomasz Gałaj

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(XentuEngine)


# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")


# help glew out a little.
set(GLEW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/thirdparty/GLEW")
set(GLEW_LIBRARY "${CMAKE_SOURCE_DIR}/lib/GLEW")


# find packages
find_package(OpenGL)
find_package(GLM)
find_package(GLFW3)
find_package(GLEW)


# source files
set(XEN_SRC_FILES
	src/Application.cpp
	src/Helper.hpp
	src/LuaEnums.hpp
	src/XentuGame.cpp
	src/XentuGame.cpp
	src/audio/AudioPlayer.cpp
	src/audio/AudioPlayer.hpp
	src/audio/Sound.cpp
	src/audio/Sound.hpp
	src/filesystem/AssetManager.cpp
	src/filesystem/AssetManager.hpp
	src/filesystem/Configuration.cpp
	src/filesystem/Configuration.hpp
	src/graphics/Batch.hpp
	src/graphics/Color.cpp
	src/graphics/Color.hpp
	src/graphics/Quad.cpp
	src/graphics/Quad.hpp
	src/graphics/Rect.cpp
	src/graphics/Rect.hpp
	src/graphics/Renderer2D.cpp
	src/graphics/Renderer2D.hpp
	src/graphics/Sprite.cpp
	src/graphics/Sprite.hpp
	src/graphics/SpriteMap.cpp
	src/graphics/SpriteMap.hpp
	src/graphics/Texture.cpp
	src/graphics/Texture.hpp
	src/graphics/Transform.cpp
	src/graphics/Transform.hpp
	src/graphics/Transformable.cpp
	src/graphics/Transformable.hpp
	src/graphics/Vector2.hpp
	src/graphics/Vertex.cpp
	src/graphics/Vertex.hpp
	src/input/InputManager.cpp
	src/input/InputManager.hpp
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
	"${CMAKE_SOURCE_DIR}/lib/GLEW/glew32.lib"
	"${CMAKE_SOURCE_DIR}/lib/GLFW3/libglfw3dll.a"
	"${CMAKE_SOURCE_DIR}/lib/lua535/liblua53.a"
)


# Define the executable
add_executable(${PROJECT_NAME} ${XEN_SRC_FILES})


# Define the link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${XEN_LIBS})