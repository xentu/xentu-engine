#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>
#include <SDL.h>

#include "Renderer.h"
#include "assets/AssetManager.h"
#include "input/InputManager.h"

namespace xen {

    class LuaMachine;

    /**
     * Utility that handles creating the window and associated Renderer instance.
     */
    class Engine {
    public:
        Engine();
        ~Engine();

        /**
         * Initialize the renderer the best possible settings.
         * @return
         */
        bool Init(int width, int height);
        bool Init(Config* config);

        /**
         * Let the engine know initial assets are loaded (game logic and config).
         * @param reloadConfig Whether to reload the config (most commonly done during an Emscripten build).
         */
        void NotifyAssetsLoaded(bool reloadConfig);

        /**
         * Process the next game frame.
         */
        void DoFrame() const;

        // call this to retrieve the loaded config.
        Config* GetConfig() const;
        // call this to retrieve the loaded renderer.
        Renderer* GetRenderer() const;
        // call this to retrieve the input (keyboard) manager.
        InputManager* GetInput() const;

        static Engine* GetInstance();

    private:
        bool m_assets_loaded = false;

        /* engine components */
        AssetManager* m_assets = nullptr;
        Config* m_config = nullptr;
        Renderer* m_renderer = nullptr;
        InputManager* m_input = nullptr;

        LuaMachine* m_machine = nullptr;

        /* sdl variables */
    	GLuint shaderProgram = 0;
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_gl_context = nullptr;

        bool m_running = false;
        bool m_size_changed = false;
        chrono::time_point<chrono::steady_clock, chrono::steady_clock::duration> m_time_start;
        chrono::nanoseconds m_time_elapsed;
        std::chrono::milliseconds m_timestep;

        /**
         * When waiting for the game to load, render a loading screen.
         */
        void DoLoadingScreen() const;

        static Engine* m_instance;
    };

} // xen

#endif //WINDOW_H