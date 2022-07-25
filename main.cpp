#include <memory>
#include <string>

//#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_timer.h"

#include "src/Xentu.h"
#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenNativeFileSystem.h"
//#include "src/machines/XentuJavaScriptMachine.h"
//#include "machines/XentuLuaMachine.h"
#include "src/machines/XentuPythonMachine.h"

using MACHINE_PTR = const std::unique_ptr<xen::XentuMachine>;
using namespace xen;


int sdl_test()
{
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    SDL_Window* win = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    int close = 0;

    // animation loop
    while (!close) {
        SDL_Event event;
         // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    // handling of close button
                    close = 1;
                    break;
            }
        }
        SDL_RenderClear(rend);
        SDL_Delay(1000 / 30);
    }
    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}


int main(int argc, char *argv[])
{
    // initialize and setup defaults for the vfs.
	vfs_initialize();
    vfs_default();
    //sdl_test();

    // load the json config.
	std::string result = vfs_get_global()->ReadAllText("/game.json");
	const XentuConfig *config = new XentuConfig(result.c_str());
    
    printf("Language: %s", config->m_language.c_str());

    if (config->m_language == "js") {
        //const MACHINE_PTR js_machine(new xen::XentuJavaScriptMachine(argc, argv, config));
        //js_machine->init("/test.js");
    }
    else if (config->m_language == "lua") {
        //const MACHINE_PTR lua_machine(new xen::XentuLuaMachine(argc, argv, config));
        //lua_machine->init("/test.lua");
    }
    else {
        MACHINE_PTR py_machine(new XentuPythonMachine(argc, argv, config));
        py_machine->init("/scripts/python/test.py");
    }

    // shutdown the filesystem.
    vfs_shutdown();

    // 3. at this point the game would have ended.
    XEN_LOG("\nEnded with code %i.", res);
    return 0;
}