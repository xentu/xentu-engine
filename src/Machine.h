#ifndef MACHINE_H
#define MACHINE_H

#include <chrono>

#include "Globals.h"
#include "Config.h"
#include "Engine.h"
#include "Renderer.h"
#include "input/InputManager.h"

namespace xen
{
    class Machine
    {
    public:
        Machine();
        virtual ~Machine();
        // initialize the machine, which runs the initial game script.
        virtual int Init() = 0;
        // triggers a game wide event.
        virtual int Trigger(const string event_name) = 0;
        // triggers a game wide event with a string arg.
        virtual int Trigger(const string event_name, const string arg0) = 0;
        // triggers a game wide event with an int arg.
        virtual int Trigger(const string event_name, const int arg0) = 0;
        // triggers a game wide event with a float arg.
        virtual int Trigger(const string event_name, const float arg0) = 0;
        // called by game code to begin the tight events loop.
        void Run();
        void RunFrame();
        // get instance of the running machine.
        static Machine* GetInstance();
        // call this to retrieve the loaded config.
        Config* GetConfig() const;
        // call this to retrieve the loaded renderer.
        Renderer* GetRenderer() const;
        // call this to retrieve the input (keyboard) manager.
        InputManager* GetInput() const;

    private:

        void HandleSdlEvents();

        bool m_running = false;
        bool m_size_changed = false;
        chrono::time_point<chrono::steady_clock, chrono::steady_clock::duration> m_time_start;
        chrono::nanoseconds m_time_elapsed;
        std::chrono::milliseconds m_timestep;

        Engine* m_engine;
        static Machine* instance;
    };
}

#endif //MACHINE_H
