#include <luna/luna.hpp>

#include "../Globals.h"
#include "../Machine.h"
#include "../vfs/XenVirtualFileSystem.h"
#include "../vfs/XenFileSystem.h"
#include "../vfs/XenZipFileSystem.h"
#include "LuaMachine.h"
#include "LuaMachineInterop.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define method(class, name, realname) {#name, &class::realname}

namespace xen
{
	XentuLuaMachineInterop::XentuLuaMachineInterop(lua_State* L)
	{
		printf("XentuLuaGame::XentuLuaGame called\n");
	}


	XentuLuaMachineInterop::~XentuLuaMachineInterop()
	{
		printf("XentuLuaGame::~XentuLuaGame called\n");
	}


	int XentuLuaMachineInterop::test(lua_State* L)
	{
		printf("Test called\n");
		return 0;
	}


	int XentuLuaMachineInterop::game_create_window(lua_State* L)
	{
		XEN_LOG("- Called game_create_window\n");
		LuaMachine* m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->Init();
		lua_pushinteger(L, 1);
		return 1;
	}

	int XentuLuaMachineInterop::game_on(lua_State* L)
	{
		XEN_LOG("- Called game_on\n");
		if (lua_isfunction(L, -1))
		{
			int functionRef = luaL_ref(L, LUA_REGISTRYINDEX);
			std::string callbackName = lua_tostring(L, -1);
			auto m = LuaMachine::GetInstance();
			m->On(callbackName, functionRef);
		}

		return 0;
	}

	int XentuLuaMachineInterop::game_trigger(lua_State* L)
	{
		XEN_LOG("- Called game_trigger\n");
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const std::string event = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		m->Trigger(event);
		return 0;
	}

	int XentuLuaMachineInterop::game_run(lua_State* L)
	{
		XEN_LOG("- Called game_run\n");
		auto m = LuaMachine::GetInstance();
		m->Run();
		return 0;
	}

	int XentuLuaMachineInterop::game_exit(lua_State* L)
	{
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Exit();
		return 0;
	}

	int XentuLuaMachineInterop::geometry_create_rect(lua_State* L)
	{
		XEN_LOG("- Called geometry_create_rect\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_mount(lua_State* L)
	{
		XEN_LOG("- Called assets_mount\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_read_text_file(lua_State* L)
	{
		XEN_LOG("- Called assets_read_text_file\n");
		return 0;
	}

	int XentuLuaMachineInterop::assets_load_texture(lua_State* L)
	{
		XEN_LOG("- Called assets_load_texture\n");
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto path = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->LoadTexture(res.buffer, res.length);
		lua_pushinteger(L, texture_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_font(lua_State* L)
	{
		XEN_LOG("- Called assets_load_font\n");
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const std::string path = lua_tostring(L, -2);
		XEN_LOG("- Attempting to read font %s\n", path.c_str());
		auto font_size = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Bytes read: %s\n", std::to_string(res.length).c_str());
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		lua_pushinteger(L, font_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_sound(lua_State* L)
	{
		XEN_LOG("- Called assets_load_sound.\n");
		auto path = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int sound_id = assets->LoadAudio(res.buffer, res.length);
		lua_pushinteger(L, sound_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_music(lua_State* L)
	{
		XEN_LOG("- Called assets_load_music.\n");
		auto path = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int music_id = assets->LoadMusic(res.buffer, res.length);
		lua_pushinteger(L, music_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_create_textbox(lua_State* L)
	{
		XEN_LOG("- Called assets_create_textbox.\n");
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		auto x = lua_tointeger(L, -4);
		auto y = lua_tointeger(L, -3);
		auto w = lua_tointeger(L, -2);
		auto h = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		int textbox_id = r->CreateTextBox(x, y, w, h);
		lua_pushinteger(L, textbox_id);
		return 1;
	}

	int XentuLuaMachineInterop::audio_play_sound(lua_State* L)
	{
		XEN_LOG("- Called audio_play_sound.\n");
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		auto sound_id = lua_tointeger(L, -3);
		auto channel = lua_tointeger(L, -2);
		auto loops = lua_tointeger(L, -1);
		AudioManager::GetInstance()->PlaySound(sound_id, channel, loops);
		return 1;
	}

	int XentuLuaMachineInterop::audio_play_music(lua_State* L)
	{
		XEN_LOG("- Called audio_play_music.\n");
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		auto music_id = lua_tointeger(L, -2);
		auto loops = lua_tointeger(L, -1);
		AudioManager::GetInstance()->PlayMusic(music_id, loops);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_stop_sound(lua_State* L)
	{
		XEN_LOG("- Called audio_stop_sound.\n");
		auto channel = lua_tointeger(L, -1);
		AudioManager::GetInstance()->StopSound(channel);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_stop_music(lua_State* L)
	{
		XEN_LOG("- Called audio_stop_music.\n");
		AudioManager::GetInstance()->StopMusic();
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_sound_volume(lua_State* L)
	{
		XEN_LOG("- Called audio_set_sound_volume.\n");
		auto sound_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetSoundVolume(sound_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_volume(lua_State* L)
	{
		XEN_LOG("- Called audio_set_channel_volume.\n");
		auto channel_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetChannelVolume(channel_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_music_volume(lua_State* L)
	{
		XEN_LOG("- Called audio_set_music_volume.\n");
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetMusicVolume(volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_panning(lua_State* L)
	{
		XEN_LOG("- Called audio_set_channel_panning.\n");
		auto channel_id = lua_tointeger(L, -3);
		float left = lua_tonumber(L, -2);
		float right = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetChannelPanning(channel_id, left, right);
		return 0;
	}
	
	int XentuLuaMachineInterop::renderer_begin(lua_State* L)
	{
		XEN_LOG("- Called renderer_begin\n");
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Begin();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_clear(lua_State* L)
	{
		XEN_LOG("- Called renderer_clear\n");
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Clear();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_present(lua_State* L)
	{
		XEN_LOG("- Called renderer_present\n");
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Present();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_texture(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texture\n");
		if (lua_gettop(L) != 5) {
			return luaL_error(L, "expecting exactly 5 arguments");
		}
		int texture_id = lua_tointeger(L, -5);
		int x = lua_tointeger(L, -4);
		int y = lua_tointeger(L, -3);
		int w = lua_tointeger(L, -2);
		int h = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTexture(texture_id, x, y, w, h);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_sub_texture(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texture\n");
		if (lua_gettop(L) != 9) {
			return luaL_error(L, "expecting exactly 9 arguments");
		}
		int texture_id = lua_tointeger(L, -9);
		int x = lua_tointeger(L, -8);
		int y = lua_tointeger(L, -7);
		int w = lua_tointeger(L, -6);
		int h = lua_tointeger(L, -5);
		int sx = lua_tointeger(L, -4);
		int sy = lua_tointeger(L, -3);
		int sw = lua_tointeger(L, -2);
		int sh = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSubTexture(texture_id, x, y, w, h, sx, sy, sw, sh);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_rectangle(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_rectangle\n");
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "expecting exactly 4 arguments");
		}
		int x = lua_tointeger(L, -4);
		int y = lua_tointeger(L, -3);
		int w = lua_tointeger(L, -2);
		int h = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawRectangle(x, y, w, h);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_textbox(lua_State* L)
	{
		XEN_LOG("- Called renderer_draw_texbox\n");
		int textbox_id = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_background(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_background\n");
		auto hex = lua_tostring(L, -1);

		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);

		printf("clear_color: %i,%i,%i (hex %s)\n", r, g, b, hex);

		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_foreground(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_foreground\n");
		auto hex = lua_tostring(L, -1);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetForegroundColor(r, g, b);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_window_mode(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_window_mode\n");
		int mode = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetWindowMode(static_cast<XenWindowMode>(mode));
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_position(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_origin\n");
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetPosition(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_origin(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_origin\n");
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetOrigin(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_rotation(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_rotation\n");
		float angle = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetRotation(angle);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_scale(lua_State* L)
	{
		XEN_LOG("- Called renderer_set_scale\n");
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetScale(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::config_get_str(lua_State* L)
	{
		XEN_LOG("- Called config_get_str\n");
		auto m_group = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_tostring(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSetting(m_group, m_name, m_default);
		lua_pushstring(L, result.c_str());
		return 1;
	}

	int XentuLuaMachineInterop::config_get_str2(lua_State* L)
	{
		XEN_LOG("- Called config_get_str2\n");
		auto m_group = lua_tostring(L, -4);
		auto m_subgroup = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_tostring(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSetting(m_group, m_subgroup, m_name, m_default);
		lua_pushstring(L, result.c_str());
		return 1;
	}

	int XentuLuaMachineInterop::config_get_bool(lua_State* L)
	{
		XEN_LOG("- Called config_get_bool\n");
		auto m_group = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_toboolean(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSettingBool(m_group, m_name, m_default);
		lua_pushboolean(L, result);
		return 1;
	}

	int XentuLuaMachineInterop::config_get_bool2(lua_State* L)
	{
		XEN_LOG("- Called config_get_bool2\n");
		auto m_group = lua_tostring(L, -4);
		auto m_subgroup = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_toboolean(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSettingBool(m_group, m_subgroup, m_name, m_default);
		lua_pushboolean(L, result);
		return 1;
	}

	int XentuLuaMachineInterop::config_get_int(lua_State* L)
	{
		XEN_LOG("- Called config_get_int\n");
		auto m_group = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSettingInt(m_group, m_name, m_default);
		lua_pushinteger(L, result);
		return 1;
	}

	int XentuLuaMachineInterop::config_get_int2(lua_State* L)
	{
		XEN_LOG("- Called config_get_bool2\n");
		auto m_group = lua_tostring(L, -4);
		auto m_subgroup = lua_tostring(L, -3);
		auto m_name = lua_tostring(L, -2);
		auto m_default = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto result = config->GetSettingInt(m_group, m_subgroup, m_name, m_default);
		lua_pushinteger(L, result);
		return 1;
	}

	int XentuLuaMachineInterop::textbox_set_text(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "expecting exactly 3 arguments");
		}
		auto textbox_id = lua_tointeger(L, -3);
		auto font_id = lua_tointeger(L, -2);
		std::string text = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxText(textbox_id, font_id, text.c_str());
		return 0;
	}

	int XentuLuaMachineInterop::textbox_set_color(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "expecting exactly 3 arguments");
		}
		auto textbox_id = lua_tointeger(L, -3);
		auto font_id = lua_tointeger(L, -2);
		auto hex = lua_tostring(L, -1);

		int rr, g, b;
		sscanf(hex, "%02x%02x%02x", &rr, &g, &b);
		printf("font_color: %i,%i,%i (hex %s)\n", rr, g, b, hex);

		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxColor(textbox_id, font_id, rr, g, b);
		return 0;
	}

	int XentuLuaMachineInterop::keyboard_key_down(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto key_code = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyDown(key_code);
		lua_pushboolean(L, down);
		return 1;
	}

	int XentuLuaMachineInterop::keyboard_key_clicked(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto key_code = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyUp(key_code);
		lua_pushboolean(L, down);
		return 1;
	}


	const char xen::XentuLuaMachineInterop::className[] = "XentuLuaMachineInterop";

	const Luna<XentuLuaMachineInterop>::PropertyType XentuLuaMachineInterop::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaMachineInterop>::FunctionType XentuLuaMachineInterop::methods[] = {
		method(XentuLuaMachineInterop, test, test),
		method(XentuLuaMachineInterop, game_create_window, game_create_window),
		method(XentuLuaMachineInterop, game_on, game_on),
		method(XentuLuaMachineInterop, game_trigger, game_trigger),
		method(XentuLuaMachineInterop, game_run, game_run),
		method(XentuLuaMachineInterop, game_exit, game_exit),
		method(XentuLuaMachineInterop, geometry_create_rect, geometry_create_rect),
		method(XentuLuaMachineInterop, assets_mount, assets_mount),
		method(XentuLuaMachineInterop, assets_read_text_file, assets_read_text_file),
		method(XentuLuaMachineInterop, assets_load_texture, assets_load_texture),
		method(XentuLuaMachineInterop, assets_load_font, assets_load_font),
		method(XentuLuaMachineInterop, assets_load_sound, assets_load_sound),
		method(XentuLuaMachineInterop, assets_load_music, assets_load_music),
		method(XentuLuaMachineInterop, assets_create_textbox, assets_create_textbox),
		method(XentuLuaMachineInterop, audio_play_sound, audio_play_sound),
		method(XentuLuaMachineInterop, audio_play_music, audio_play_music),
		method(XentuLuaMachineInterop, audio_stop_sound, audio_stop_sound),
		method(XentuLuaMachineInterop, audio_stop_music, audio_stop_music),
		method(XentuLuaMachineInterop, audio_set_sound_volume, audio_set_sound_volume),
		method(XentuLuaMachineInterop, audio_set_channel_volume, audio_set_channel_volume),
		method(XentuLuaMachineInterop, audio_set_music_volume, audio_set_music_volume),
		method(XentuLuaMachineInterop, audio_set_channel_panning, audio_set_channel_panning),
		method(XentuLuaMachineInterop, renderer_begin, renderer_begin),
		method(XentuLuaMachineInterop, renderer_clear, renderer_clear),
		method(XentuLuaMachineInterop, renderer_present, renderer_present),
		method(XentuLuaMachineInterop, renderer_draw_texture, renderer_draw_texture),
		method(XentuLuaMachineInterop, renderer_draw_sub_texture, renderer_draw_sub_texture),
		method(XentuLuaMachineInterop, renderer_draw_rectangle, renderer_draw_rectangle),
		method(XentuLuaMachineInterop, renderer_draw_textbox, renderer_draw_textbox),
		method(XentuLuaMachineInterop, renderer_set_background, renderer_set_background),
		method(XentuLuaMachineInterop, renderer_set_foreground, renderer_set_foreground),
		method(XentuLuaMachineInterop, renderer_set_window_mode, renderer_set_window_mode),
		method(XentuLuaMachineInterop, renderer_set_position, renderer_set_position),
		method(XentuLuaMachineInterop, renderer_set_origin, renderer_set_origin),
		method(XentuLuaMachineInterop, renderer_set_rotation, renderer_set_rotation),
		method(XentuLuaMachineInterop, renderer_set_scale, renderer_set_scale),
		method(XentuLuaMachineInterop, config_get_str, config_get_str),
		method(XentuLuaMachineInterop, config_get_str2, config_get_str2),
		method(XentuLuaMachineInterop, config_get_bool, config_get_bool),
		method(XentuLuaMachineInterop, config_get_bool2, config_get_bool2),
		method(XentuLuaMachineInterop, config_get_int, config_get_int),
		method(XentuLuaMachineInterop, config_get_int2, config_get_int2),
		method(XentuLuaMachineInterop, textbox_set_text, textbox_set_text),
		method(XentuLuaMachineInterop, textbox_set_color, textbox_set_color),
		method(XentuLuaMachineInterop, keyboard_key_down, keyboard_key_down),
		method(XentuLuaMachineInterop, keyboard_key_clicked, keyboard_key_clicked),
		{0,0}
	};
}