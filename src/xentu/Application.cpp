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
#include "Helper.h"

extern "C" {
#include "lua53/lua.h"
#include "lua53/lauxlib.h"
#include "lua53/lualib.h"
}

#pragma region Resources

struct Res { const char* data; size_t size; };

extern "C" Res RES_SHADER_VERTEX(void);
extern "C" Res RES_SHADER_FRAGMENT(void);
extern "C" Res LUA_STARTUP(void);
extern "C" Res LUA_CLASSES(void);
extern "C" Res LUA_BEFORE_INIT(void);

Res res_shader_vertex = RES_SHADER_VERTEX();
Res res_shader_fragment = RES_SHADER_FRAGMENT();
Res lua_startup = LUA_STARTUP(); // use *.data, *.size
Res lua_classes = LUA_CLASSES();
Res lua_before_init = LUA_BEFORE_INIT();

// convert the raw data into standard strings. The _size is important as the raw data is null terminated.
const std::string m_shader_vertex(res_shader_vertex.data, res_shader_vertex.size);
const std::string m_shader_fragment(res_shader_fragment.data, res_shader_fragment.size);
const std::string m_xen_startup_lua(lua_startup.data, lua_startup.size);
const std::string m_xen_startup_lua_classes(lua_classes.data, lua_classes.size);
const std::string m_xen_startup_lua_before_init(lua_before_init.data, lua_before_init.size);

#pragma endregion


using namespace std::chrono_literals;


/// <summary>
/// The main entry point.
/// </summary>
int main(int arg_count, char* args[])
{
	// see if we need to use proxy path mode (aka via the sdk).
	if (arg_count > 1) {
		std::string command_arg(args[1]);
		if (command_arg == "proxy") {
			std::cout << "Set USE_PROXY_PATH" << std::endl;
			xen::XentuGame::USE_PROXY_PATH = true;
		}
	}

	std::cout << "Xentu Game Engine v" << XEN_ENGINE_VERSION << std::endl;
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
	Luna<xen::KeyboardManager>::Register(L, false);
	Luna<xen::MouseManager>::Register(L, false);
	Luna<xen::Viewport>::Register(L, false);

	/* core lua ran before anything else inc standard libraries. */
	luaL_openlibs(L);
	luaL_dostring(L, m_xen_startup_lua.c_str());
	auto ret = luaL_dostring(L, m_xen_startup_lua_classes.c_str());
	if (ret != LUA_OK) {
		printf("Error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1); // pop error message
		return false;
	}

	/* grab a pointer to the game singleton instance */
	xen::XentuGame* game = xen::XentuGame::get_instance(L);
	game->pre_init();
	std::string game_path = game->get_base_path() + "/Game.lua";

	/* audio test */
	//xen::AudioPlayer* audio = new xen::AudioPlayer();
	//xen::Sound* sound = new xen::Sound(base_path + "../data/Audio/Frontier_Rem1.mp3");
	//audio->play(sound);

	if (luaL_dofile(L, game_path.c_str()) == LUA_OK)
	{
		unsigned int ms = 1000.0f / game->config->m_update_frequency;
		std::chrono::milliseconds timestep(ms);

		/* initialize the game. */
		if (result = game->initialize(L, m_shader_vertex, m_shader_fragment) == 0)
		{
			luaL_dostring(L, m_xen_startup_lua_before_init.c_str());

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