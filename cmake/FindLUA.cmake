# Locate the lua library
#
# This module defines the following variables:
#
# LUA_LIBRARY the name of the library;

# Cmake file inspired by: https://github.com/daw42/glslcookbook

set( _lua_LIB_SEARCH_DIRS
"/usr/lib"
"/usr/local/lib"
"/usr/lib/x86_64-linux-gnu/"
"${CMAKE_SOURCE_DIR}/lib/lua535/" )

# Search for the library
FIND_LIBRARY(LUA_LIBRARY liblua53 liblua5.3.so.0 PATHS ${_lua_LIB_SEARCH_DIRS} )
message("-- LUA_LIBRARY = ${LUA_LIBRARY}")

# Finish up
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(lua DEFAULT_MSG LUA_LIBRARY)