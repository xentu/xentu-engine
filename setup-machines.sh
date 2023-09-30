#!/bin/sh

# Compiles the embeded machine script files into .h files before engine is built.


# -----------------------------------------------------------------------------
XEN_JS_FILE='./src/machines/JavaScriptMachineScripts.h'
XEN_JS_FILE_IN="${XEN_JS_FILE}.js"
XEN_JS_FILE_SCENES_IN="${XEN_JS_FILE}.Scenes.js"

rm $XEN_JS_FILE 2> /dev/null
cat > $XEN_JS_FILE <<- EOM
#pragma once
/**
 * This file holds the javascript code to be executed when a js engine is
 * loaded by the user.
 */
namespace xen { const char * xen_js_script_init = R"(
EOM
cat $XEN_JS_FILE_IN >> $XEN_JS_FILE
echo -e '\n' >> $XEN_JS_FILE
cat $XEN_JS_FILE_SCENES_IN >> $XEN_JS_FILE
echo -e '\n)"; }' >> $XEN_JS_FILE
echo "${XEN_JS_FILE} updated."


# -----------------------------------------------------------------------------
XEN_LUA_FILE='./src/machines/LuaMachineScript.h'
XEN_LUA_FILE_IN="${XEN_LUA_FILE}.lua"
XEN_LUA_FILE_SCENES_IN="${XEN_LUA_FILE}.Scenes.lua"

rm $XEN_LUA_FILE 2> /dev/null
cat > $XEN_LUA_FILE <<- EOM
#pragma once
/**
 * This file holds the lua code to be executed when a lua engine is loaded
 * by the user. 
 */
namespace xen { const char * xen_lua_script_init = R"(
EOM
cat $XEN_LUA_FILE_IN >> $XEN_LUA_FILE
echo -e '\n' >> $XEN_LUA_FILE
cat $XEN_LUA_FILE_SCENES_IN >> $XEN_LUA_FILE
echo -e '\n)"; }' >> $XEN_LUA_FILE
echo "${XEN_LUA_FILE} updated."


# -----------------------------------------------------------------------------
XEN_PY_FILE='./src/machines/PythonMachineScripts.h'
XEN_PY_FILE_IN="${XEN_PY_FILE}.py"
XEN_PY_FILE_SCENES_IN="${XEN_PY_FILE}.Scenes.py"

rm $XEN_PY_FILE 2> /dev/null
cat > $XEN_PY_FILE <<- EOM
#pragma once
#if XEN_PY

/**
 * This file holds the python code to be executed when a python engine is loaded
 * by the user. The file does the following:
 * 
 * - Inject an extra loader into importlib that uses the engines vfs.
 * - Add py exclusive modules (const, geometry) under the xentu dummy module.
 * - Add aliases for c++ modules like vfs under the xentu dummy module.
 */
namespace xen { const char * xen_py_script_init = R"(
EOM
cat $XEN_PY_FILE_IN >> $XEN_PY_FILE
echo -e '\n' >> $XEN_PY_FILE
cat $XEN_PY_FILE_SCENES_IN >> $XEN_PY_FILE
echo -e '\n)"; }\n#endif' >> $XEN_PY_FILE

echo "${XEN_PY_FILE} updated."