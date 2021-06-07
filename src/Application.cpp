#ifndef LUASCRIPT_H
#define LUASCRIPT_H
#endif

#define LUA_OK 0

#include <iostream>
#include <chrono>
#include <string>
#include <thread>

#include <luna/luna.hpp>
#include "XentuGame.h"
#include "StartupLua.h"

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
    Luna<xen::Viewport>::Register(L, false);

    /* core lua ran before anything else inc standard libraries. */
    luaL_openlibs(L);
    luaL_dostring(L, m_xen_startup_lua);
    auto ret = luaL_dostring(L, m_xen_startup_lua_classes);
    if (ret != LUA_OK) {
      printf("Error: %s\n", lua_tostring(L, -1));
      lua_pop(L, 1); // pop error message
      return false;
    }

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
            luaL_dostring(L, m_xen_startup_lua_before_init);

            /* test the event system */
            game->trigger(L, "init");

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