# Locate the glfw3 library
#
# This module defines the following variables:
#
# GLFW3_LIBRARY the name of the library;
# GLFW3_INCLUDE_DIR where to find glfw include files.
# GLFW3_FOUND true if both the GLFW3_LIBRARY and GLFW3_INCLUDE_DIR have been found.

# Cmake file inspired by: https://github.com/daw42/glslcookbook

set( _glfw3_HEADER_SEARCH_DIRS
"/usr/include"
"/usr/local/include"
"${CMAKE_SOURCE_DIR}/thirdparty/"
"C:/Program Files (x86)/glfw/include" )

set( _glfw3_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"/usr/lib/x86_64-linux-gnu/"
"${CMAKE_SOURCE_DIR}/lib/GLFW3/"
"C:/Program Files (x86)/glfw/lib-msvc110" )

# Search for the header
FIND_PATH(GLFW3_INCLUDE_DIR "GLFW3/glfw3.h" PATHS ${_glfw3_HEADER_SEARCH_DIRS} )
message("-- GLFW3_INCLUDE_DIR = ${GLFW3_INCLUDE_DIR}")

# Search for the library
FIND_LIBRARY(GLFW3_LIBRARY "libglfw3dll.a" "libglfw.so.3.3" PATHS ${_glfw3_LIB_SEARCH_DIRS} )
message("-- GLFW3_LIBRARY = ${GLFW3_LIBRARY}")

# Finish up
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(glfw3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)