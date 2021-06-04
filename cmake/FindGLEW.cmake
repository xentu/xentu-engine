# Locate the glfw3 library
#
# This module defines the following variables:
#
# GLEW_LIBRARY the name of the library;
# GLEW_INCLUDE_DIR where to find glfw include files.
# GLEW_FOUND true if both the GLEW_LIBRARY and GLEW_INCLUDE_DIR have been found.

# Cmake file inspired by: https://github.com/daw42/glslcookbook

set( _glew_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/thirdparty/"
"C:/Program Files (x86)/glew/include" )

set( _glew_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"${CMAKE_SOURCE_DIR}/lib/GLEW/"
"C:/Program Files (x86)/glfw/lib-msvc110" )

# Search for the header
FIND_PATH(GLEW_INCLUDE_DIR "GLEW/GL/glew.h" PATHS ${_glew_HEADER_SEARCH_DIRS} )
message("-- GLEW_INCLUDE_DIR = ${GLEW_INCLUDE_DIR}")

# Search for the library
FIND_LIBRARY(GLEW_LIBRARY glew32 PATHS ${_glew_LIB_SEARCH_DIRS} )

# Finish up
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(glew DEFAULT_MSG GLEW_LIBRARY GLEW_INCLUDE_DIR)