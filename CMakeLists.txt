# Copyright (C) 2021 Kodaloid Soft
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(xentu VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(OpenGL_GL_PREFERENCE GLVND) #LEGACY

find_package(LUA)
find_package(Embed REQUIRED)
find_package(OpenGL)
find_package(GLM)
find_package(GLEW)
find_package(GLFW3)


# include resources
EMBED_TARGET(RES_SHADER_VERTEX "src/xentu/resources/VertexShader.shader")
EMBED_TARGET(RES_SHADER_FRAGMENT "src/xentu/resources/FragmentShader.shader")
EMBED_TARGET(LUA_STARTUP "src/xentu/resources/startup.lua")
EMBED_TARGET(LUA_BEFORE_INIT "src/xentu/resources/before_init.lua")


# set compiler specific flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
	# using Clang
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/clang")
	set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/clang")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	# using GCC
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/gcc")
	set(RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/build/gcc")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  	# using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# using MSVCC
	set(GLFW3_LIBRARY "${CMAKE_SOURCE_DIR}/lib/GLFW3/libglfw3dll.a")
	set(LUA_LIBRARY "${CMAKE_SOURCE_DIR}/lib/lua535/lua53.lib")
endif()
message("--COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")


# source files for engine
set(XEN_ENGINE_FILES
	${EMBED_RES_SHADER_VERTEX_OUTPUTS}
	${EMBED_RES_SHADER_FRAGMENT_OUTPUTS}
	${EMBED_LUA_STARTUP_OUTPUTS}
	${EMBED_LUA_CLASSES_OUTPUTS}
	${EMBED_LUA_BEFORE_INIT_OUTPUTS}
	src/xentu/Application.cpp
	src/xentu/XentuGame.h
	src/xentu/XentuGame.cpp
	src/xentu/XentuScene.h
	src/xentu/XentuScene.cpp
	src/xentu/audio/AudioPlayer.cpp
	src/xentu/audio/AudioPlayer.h
	src/xentu/audio/Sound.cpp
	src/xentu/audio/Sound.h
	src/xentu/filesystem/AssetManager.cpp
	src/xentu/filesystem/AssetManager.h
	src/xentu/filesystem/Configuration.cpp
	src/xentu/filesystem/Configuration.h
	src/xentu/graphics/Batch.h
	src/xentu/graphics/Color.cpp
	src/xentu/graphics/Color.h
	src/xentu/graphics/Rect.cpp
	src/xentu/graphics/Rect.h
	src/xentu/graphics/Renderer2D.cpp
	src/xentu/graphics/Renderer2D.h
	src/xentu/graphics/Sprite.cpp
	src/xentu/graphics/Sprite.h
	src/xentu/graphics/SpriteMap.cpp
	src/xentu/graphics/SpriteMap.h
	src/xentu/graphics/Texture.cpp
	src/xentu/graphics/Texture.h
	src/xentu/graphics/TileMap.cpp
	src/xentu/graphics/TileMap.h
	src/xentu/graphics/TileMapLayer.cpp
	src/xentu/graphics/TileMapLayer.h
	src/xentu/graphics/Transform.cpp
	src/xentu/graphics/Transform.h
	src/xentu/graphics/Transformable.cpp
	src/xentu/graphics/Transformable.h
	src/xentu/graphics/Vector2.h
	src/xentu/graphics/Vector3.h
	src/xentu/graphics/Vector4.h
	src/xentu/graphics/Vertex.cpp
	src/xentu/graphics/Vertex.h
	src/xentu/graphics/Viewport.h
	src/xentu/graphics/Viewport.cpp
	src/xentu/input/MouseManager.cpp
	src/xentu/input/MouseManager.h
	src/xentu/input/KeyboardManager.cpp
	src/xentu/input/KeyboardManager.h
	src/xentu/utilities/Advisor.cpp
	src/xentu/utilities/Advisor.h
	src/xentu/utilities/Helper.h


	src/xentu/libraries/tmxlite/detail/Android.hpp
	src/xentu/libraries/tmxlite/detail/Log.hpp
	src/xentu/libraries/tmxlite/detail/pugiconfig.hpp
	src/xentu/libraries/tmxlite/detail/pugixml.cpp
	src/xentu/libraries/tmxlite/detail/pugixml.hpp
	src/xentu/libraries/tmxlite/Config.hpp
	src/xentu/libraries/tmxlite/FreeFuncs.cpp
	src/xentu/libraries/tmxlite/FreeFuncs.hpp
	src/xentu/libraries/tmxlite/ImageLayer.cpp
	src/xentu/libraries/tmxlite/ImageLayer.hpp
	src/xentu/libraries/tmxlite/Layer.hpp
	src/xentu/libraries/tmxlite/LayerGroup.cpp
	src/xentu/libraries/tmxlite/Map.cpp
	src/xentu/libraries/tmxlite/Map.hpp
	src/xentu/libraries/tmxlite/miniz.h
	src/xentu/libraries/tmxlite/miniz.c
	src/xentu/libraries/tmxlite/Object.cpp
	src/xentu/libraries/tmxlite/Object.hpp
	src/xentu/libraries/tmxlite/ObjectGroup.cpp
	src/xentu/libraries/tmxlite/ObjectGroup.hpp
	src/xentu/libraries/tmxlite/ObjectTypes.cpp
	src/xentu/libraries/tmxlite/ObjectTypes.hpp
	src/xentu/libraries/tmxlite/Property.cpp
	src/xentu/libraries/tmxlite/Property.hpp
	src/xentu/libraries/tmxlite/TileLayer.cpp
	src/xentu/libraries/tmxlite/TileLayer.hpp
	src/xentu/libraries/tmxlite/Tileset.cpp
	src/xentu/libraries/tmxlite/Tileset.hpp
	src/xentu/libraries/tmxlite/Types.hpp
	src/xentu/libraries/tmxlite/Types.inl
)


# Define the include DIRs
include_directories(
	"${CMAKE_SOURCE_DIR}/src/xentu"
	"${CMAKE_SOURCE_DIR}/src/xentu/libraries"
	"${CMAKE_SOURCE_DIR}/src/xentusdk/libraries"
	"${CMAKE_SOURCE_DIR}/thirdparty"
)


# compile list of libs (that will be linked to the exe)
if (WIN32)
    message("-- LIB MODE: Win32")
	set(XEN_LIBS
		"${OPENGL_LIBRARIES}"
		"${GLEW_LIBRARY}"
		"${GLFW3_LIBRARY}"
		"${LUA_LIBRARY}"
	)
elseif (APPLE)
	message("-- LIB MODE: MacOS")
	set(XEN_LIBS
		m
		dl
		"${OPENGL_LIBRARIES}"
		"${GLEW_LIBRARY}"
		"${GLFW3_LIBRARY}"
		"${LUA_LIBRARY}"
		pthread
	)
elseif (UNIX)
    message("-- LIB MODE: Unix")
	set(XEN_LIBS
		dl
		"${OPENGL_LIBRARIES}"
		"${GLEW_LIBRARY}"
		"${GLFW3_LIBRARY}"
		"${LUA_LIBRARY}"
		pthread
	)
endif (WIN32)


# Define the executable
add_executable(xentu ${XEN_ENGINE_FILES})
#set_target_properties(xentu PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32") -- forces 32 bit
target_compile_definitions(xentu PRIVATE XEN_ENGINE_VERSION="${CMAKE_PROJECT_VERSION}")


# Define the link libraries
target_link_libraries(xentu PRIVATE ${XEN_LIBS})


# --------------------------------------------------------------------------------------------------


# Define the sdk executable
project(xentusdk VERSION 0.0.1)

# source files for sdk
set(XEN_SDK_FILES
	src/xentusdk/main.cpp
)

add_executable(xentusdk ${XEN_SDK_FILES})
target_compile_definitions(xentusdk PRIVATE XEN_SDK_VERSION="${CMAKE_PROJECT_VERSION}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()


# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")