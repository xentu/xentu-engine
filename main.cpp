#include <memory>
#include <string>

//#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_timer.h"

#include "src/Xentu.h"
#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenNativeFileSystem.h"
#include "src/machines/XentuJavaScriptMachine.h"
#include "src/machines/XentuLuaMachine.h"
#include "src/machines/XentuPythonMachine.h"

using MACHINE_PTR = const std::unique_ptr<xen::XentuMachine>;

using namespace xen;


int main(int argc, char *argv[])
{
    int res = 0;
    // initialize and setup defaults for the vfs.
	vfs_initialize();
    vfs_default();

    // load the json config.
	std::string result = vfs_get_global()->ReadAllText("/game.json");
	XentuConfig *config = new XentuConfig(result.c_str());
    
    printf("Language: %s\n", config->language.c_str());

    if (config->language == "javascript") {
        MACHINE_PTR js_machine(new xen::XentuJavaScriptMachine(argc, argv, config));
        res = js_machine->init();
    }
    else if (config->language == "lua") {
        MACHINE_PTR lua_machine(new XentuLuaMachine(argc, argv, config));
        res = lua_machine->init();
    }
    else {
        MACHINE_PTR py_machine(new XentuPythonMachine(argc, argv, config));
        res = py_machine->init();
    }

    // dispose resources.
    delete config;
    vfs_shutdown();

    // 3. at this point the game would have ended.
    XEN_LOG("\nEnded with code %i.", res);
    return res;
}