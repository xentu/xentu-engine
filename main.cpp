#include <memory>
#include <string>

//#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_timer.h>

#include "src/Globals.h"
#include "src/Config.h"
#include "src/Machine.h"
#include "src/Exceptions.h"
#include "src/vfs/XenVirtualFileSystem.h"
#include "src/vfs/XenFileSystem.h"
#include "src/vfs/XenNativeFileSystem.h"
#include "src/machines/JavaScriptMachine.h"
#include "src/machines/LuaMachine.h"
#include "src/machines/PythonMachine.h"

using MACHINE_PTR = const std::unique_ptr<xen::XentuMachine>;

using namespace xen;


int main(int argc, char *argv[])
{
    int res = 0;
    // initialize and setup defaults for the vfs.
	vfs_initialize();
    vfs_default();

    // load the json config.
    std::string result;

    try {
	    result = vfs_get_global()->ReadAllText("/game.json");
    }
    catch (XentuNotFoundException e) {
        vfs_shutdown();
        return XEN_ERROR("- Error unable to locate %s\n", e.what());
    }

	XentuConfig *config = new XentuConfig(result.c_str());
    
    printf("Language: %s\n", config->language.c_str());

    if (config->language == "javascript") {
        MACHINE_PTR js_machine(new xen::JavaScriptMachine(argc, argv, config));
        res = js_machine->Init();
    }
    else if (config->language == "lua") {
        MACHINE_PTR lua_machine(new LuaMachine(argc, argv, config));
        res = lua_machine->Init();
    }
    else {
        MACHINE_PTR py_machine(new PythonMachine(argc, argv, config));
        res = py_machine->Init();
    }

    // dispose resources.
    delete config;
    vfs_shutdown();

    // 3. at this point the game would have ended.
    XEN_LOG("\nEnded with code %i.", res);
    return res;
}