#include <iostream>
#include <fstream>

#include "Constants.h"
#include "Globals.h"
#include "machines/LuaMachine.h"
#include "Engine.h"
#include "assets/DataManager.h"

namespace xen {


    Engine::Engine()
    { }


    bool Engine::Init(const int width, const int height) {
        //XEN_DEBUG("Calling Engine::Init()");
        m_assets = new AssetManager();
        m_config = new Config(width, height);
        m_renderer = new Renderer(m_config);
        m_renderer->Init();
        m_renderer->NewFrame(true);
        m_renderer->SetClearColor(255, 127, 255);
        m_renderer->SetBlend(true);
        m_input = new InputManager();
        m_instance = this;
        return true;
    }


    bool Engine::Init(Config* config) {
        m_assets = new AssetManager();
        m_config = config;
        m_renderer = new Renderer(m_config);
        m_renderer->Init();
        m_renderer->NewFrame(true);
        m_renderer->SetClearColor(255, 127, 255);
        m_renderer->SetBlend(true);
        m_input = new InputManager();
        m_instance = this;
        return true;

    }


    Engine* Engine::m_instance = nullptr;
    Engine* Engine::GetInstance()	{
        if (m_instance == nullptr) {
            XEN_ERROR("Error, called DataManager::GetInstance() before initialization!");
            exit(123);
        }
        return m_instance;
    }


    void Engine::NotifyAssetsLoaded(bool reloadConfig) {
        if (reloadConfig) {
            std::ifstream file("resources/game.json", std::ios::binary);
            if (file.is_open()) {
                std::cout << "> Success! Found game.json in virtual FS." << std::endl;

                std::ostringstream buffer;
                buffer << file.rdbuf();
                std::string config_json = buffer.str();

                XEN_ECHO("> Loading config from game.json\n");

                m_config = new Config(config_json.c_str());

                Viewport vp = Viewport(m_config->viewport.width, m_config->viewport.height, m_config->viewport.mode);

                m_renderer->ResizeWindow(m_config->window.width, m_config->window.height);
                m_renderer->ResizeViewport(vp);

                // todo: apply config settings to game here before the machine loads.

                m_machine = new LuaMachine();
                m_machine->Init();

                XEN_ECHO("> Found entry point\n");
                m_assets_loaded = true;
            } else {
                std::cout << "C++: Error! Could not find game.json." << std::endl;
            }
        }
        else {
            m_machine = new LuaMachine();
            m_machine->Init();

            XEN_ECHO("> Found entry point\n");
            m_assets_loaded = true;
        }
    }


    void Engine::DoFrame() const {
        if (!m_assets_loaded || m_machine == nullptr) {
            m_renderer->NewFrame(false);
            this->DoLoadingScreen();
        }
        else {
            // tell machine to do frame.
            m_machine->RunFrame();
        }
    }


    Config* Engine::GetConfig() const {
        return m_config;
    }

    Renderer* Engine::GetRenderer() const {
        return m_renderer;
    }

    InputManager* Engine::GetInput() const {
        return m_input;
    }


    void Engine::DoLoadingScreen() const {
        m_renderer->Clear();
        m_renderer->Begin(true);
        m_renderer->SetForegroundColor(255, 127, 255);
        m_renderer->DrawRectangle(10, 10, 32, 32);

        m_renderer->Begin(true);
        m_renderer->SetForegroundColor(127, 127, 127);
        m_renderer->DrawRectangle(52, 10, 32, 64);

        m_renderer->Present();
    }


    Engine::~Engine() {
        SDL_DestroyWindow(m_window);
        m_instance = nullptr;
    }


} // xen