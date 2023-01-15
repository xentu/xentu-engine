#include <memory>
#include <string>

//#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_timer.h>
#if XEN_DEBUG
#include <rang.h>
#endif

#include "src/Globals.h"
#include "src/Config.h"
#include "src/Machine.h"
#include "src/Exceptions.h"
#include "src/StringUtils.h"
#include "src/vfs/XenVirtualFileSystem.h"
#include "src/vfs/XenFileSystem.h"
#include "src/vfs/XenNativeFileSystem.h"
#include "src/machines/JavaScriptMachine.h"
#include "src/machines/LuaMachine.h"
#if XEN_PY
#include "src/machines/PythonMachine.h"
#endif

using MACHINE_PTR = const std::unique_ptr<xen::Machine>;

using namespace xen;


int main(int argc, char *argv[])
{
    int res = 0;


    #if XEN_DEBUG
    setbuf(stdout, NULL);
    cout << endl << rang::fg::reset << "#########################################" << endl;
    cout << rang::fg::reset << "#" << rang::fg::yellow << " Xentu Game Engine" << rang::fg::yellow << " - v" << XEN_VER << rang::fg::reset << endl;
    cout << rang::fg::reset << "#########################################" << rang::fg::reset << endl << endl;

    // check for console switches if using the _debug version of the engine binary.
    if (argc > 1) {
        string argv1 = argv[1];
        if (argv1 == "--version" || argv1 == "-v") {
            XEN_ECHO("Version: %s\n", XEN_VER);
            return res;
        }
    }
    #endif

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

	Config *config = new Config(result.c_str());
    AudioManager *audio = new AudioManager(config);
    AssetManager *assets = new AssetManager();
    
    if (StringUtils::EndsWith(config->entry_point, "js")) {
        printf("> Language: JavaScript\n");
        MACHINE_PTR js_machine(new xen::JavaScriptMachine(argc, argv, config));
        res = js_machine->Init();
    }
    else if (StringUtils::EndsWith(config->entry_point, "lua")) {
        printf("> Language: LUA\n");
        MACHINE_PTR lua_machine(new LuaMachine(argc, argv, config));
        res = lua_machine->Init();
    }
    #if XEN_PY
    else if (StringUtils::EndsWith(config->entry_point, "py")) {
        printf("> Language: Python\n");
        MACHINE_PTR py_machine(new PythonMachine(argc, argv, config));
        res = py_machine->Init();
    }
    #endif
    else {
        XEN_ERROR("Machine platform not currently supported, please use a recognised file format for the entry point.");
    }

    // dispose resources.
    delete assets;
    delete audio;
    delete config;
    vfs_shutdown();

    // 3. at this point the game would have ended.
    XEN_ECHO("\nEnded with code %i.", res);
    return res;
}