#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include <fstream>

#include "src/Config.h"
#include "src/Engine.h"
#include "src/Globals.h"


xen::Engine* engine = nullptr;


void mainLoop() {
    engine->DoFrame();
}


#ifdef __EMSCRIPTEN__
extern "C" {
    void EMSCRIPTEN_KEEPALIVE notify_assets_ready() {
        XEN_DEBUG("> Assets are now loaded! Switching to game mode.\n");
        engine->NotifyAssetsLoaded(true);
    }
}
#endif


int main(int argc, char** argv) {
    engine = new xen::Engine();


#ifdef __EMSCRIPTEN__
    if (engine->Init(640, 480)) {
        emscripten_set_main_loop(mainLoop, 0, true);
    }
#else

    std::ifstream file("resources/game.json", std::ios::binary);
    if (file.is_open()) {
        std::cout << "> Success! Found game.json in virtual FS." << std::endl;

        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string config_json = buffer.str();
        XEN_ECHO("> Loading config from game.json\n");
        auto* config = new xen::Config(config_json.c_str());

        if (engine->Init(config)) {
            printf("Hello");
            bool running = true;
            engine->NotifyAssetsLoaded(false);
            SDL_Event event;
            while (running) {
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        running = false;
                    }
                }
                mainLoop();
            }
        }
    }
    else {
        std::cout << "C++: Error! Could not find game.json." << std::endl;
    }

#endif

    delete engine;
    return 0;
}