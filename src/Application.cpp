#ifndef LUASCRIPT_H
#define LUASCRIPT_H
#endif

#define LUA_OK 0

#include <iostream>
#include <chrono>
#include <string>
#include <thread>

#include <luna/luna.hpp>
#include "XentuGame.hpp"
#include "LuaEnums.hpp"

extern "C" {
#include "lua53/lua.h"
#include "lua53/lauxlib.h"
#include "lua53/lualib.h"
}


using namespace std::chrono_literals;


/// <summary>
/// The main entry point.
/// </summary>
int main(void)
{
    using clock = std::chrono::high_resolution_clock;

    int result = 0;
    lua_State* L = luaL_newstate();

    /* initialize variables for timing */
    auto time_start = clock::now();
    std::chrono::nanoseconds time_elapsed(0ns);

    /* make luna aware of classes it can proxy to lua. */
    Luna<xen::XentuGame>::Register(L, false);
    Luna<xen::AssetManager>::Register(L, false);
    Luna<xen::Renderer2D>::Register(L, false);
    Luna<xen::AudioPlayer>::Register(L, false);
    Luna<xen::InputManager>::Register(L, false);

    /* core lua ran before anything else inc standard libraries. */
    luaL_openlibs(L);
    luaL_dostring(L, m_xen_lua_enums);

    /* load and our Game.lua script */
    std::string base_path = xen::XentuGame::get_current_directory();
    std::string game_path = base_path + "../data/Game.lua";

    /* audio test */
    //xen::AudioPlayer* audio = new xen::AudioPlayer();
    //xen::Sound* sound = new xen::Sound(base_path + "../data/Audio/Frontier_Rem1.mp3");
    //audio->play(sound);

    if (luaL_dofile(L, game_path.c_str()) == LUA_OK)
    {
        /* grab a pointer to the game singleton instance */
        xen::XentuGame* game = xen::XentuGame::get_instance(L);

        unsigned int ms = 1000.0f / game->config->m_update_frequency;
        std::chrono::milliseconds timestep(ms);

        /* initialize the game. */
        if (result = game->initialize(L) == 0)
        {
            /* test the event system */
            game->trigger(L, "initialized");

            /* Loop until the user closes the window */
            while (game->is_running())
            {
                game->poll_input();
                auto delta_time = clock::now() - time_start;
                time_start = clock::now();
                time_elapsed += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

                // update game logic as lag permits
                while (time_elapsed >= timestep) {
                    time_elapsed -= timestep;
                    game->update(L); // update at a fixed rate each time
                }

                game->draw(L);
            }
        }

        delete game;
    }
    else
    {
        std::string errormsg = lua_tostring(L, -1);
        std::cout << errormsg << std::endl;
    }

    //delete audio;

    lua_close(L);
    //delete sound;
    //delete am;

    return result;
}