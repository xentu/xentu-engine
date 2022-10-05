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
	{ }


	XentuLuaMachineInterop::~XentuLuaMachineInterop()
	{ }


	#pragma region Game Object

	int XentuLuaMachineInterop::game_create_window(lua_State* L)
	{
		LuaMachine* m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->Init();
		lua_pushinteger(L, 1);
		return 1;
	}

	int XentuLuaMachineInterop::game_on(lua_State* L)
	{
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

	#pragma endregion


	#pragma region Assets

	int XentuLuaMachineInterop::assets_mount(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		auto s_point = lua_tostring(L, -2);
		auto s_path = lua_tostring(L, -1);
		// create the file system mount & init.
		XenFileSystemPtr zip_fs(new XenZipFileSystem(s_path, "/"));
		zip_fs->Initialize();
		// add the file systems to the vfs.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
    	vfs->AddFileSystem(s_point, zip_fs);
		return 0;
	}

	int XentuLuaMachineInterop::assets_read_text_file(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto path = lua_tostring(L, -1);
		auto res = vfs_get_global()->ReadAllText(path);
		lua_pushstring(L, res.c_str());
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_texture(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto path = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int texture_id = r->LoadTexture(res.buffer, res.length);
		lua_pushinteger(L, texture_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_font(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const std::string path = lua_tostring(L, -2);
		auto font_size = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		lua_pushinteger(L, font_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_sound(lua_State* L)
	{
		auto path = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int sound_id = assets->LoadAudio(res.buffer, res.length);
		lua_pushinteger(L, sound_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_music(lua_State* L)
	{
		auto path = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int music_id = assets->LoadMusic(res.buffer, res.length);
		lua_pushinteger(L, music_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_shader(lua_State* L)
	{
		auto vertex_src = lua_tostring(L, -2);
		auto frag_src = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		int asset_id = assets->LoadShader(vertex_src, frag_src);
		lua_pushinteger(L, asset_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_create_textbox(lua_State* L)
	{
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

	#pragma endregion


	#pragma region Audio

	int XentuLuaMachineInterop::audio_play_sound(lua_State* L)
	{
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
		auto channel = lua_tointeger(L, -1);
		AudioManager::GetInstance()->StopSound(channel);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_stop_music(lua_State* L)
	{
		AudioManager::GetInstance()->StopMusic();
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_sound_volume(lua_State* L)
	{
		auto sound_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetSoundVolume(sound_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_volume(lua_State* L)
	{
		auto channel_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetChannelVolume(channel_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_music_volume(lua_State* L)
	{
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetMusicVolume(volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_panning(lua_State* L)
	{
		auto channel_id = lua_tointeger(L, -3);
		float left = lua_tonumber(L, -2);
		float right = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetChannelPanning(channel_id, left, right);
		return 0;
	}
	
	#pragma endregion


	#pragma region Renderer

	int XentuLuaMachineInterop::renderer_begin(lua_State* L)
	{
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Begin();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_clear(lua_State* L)
	{
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Clear();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_present(lua_State* L)
	{
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Present();
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_texture(lua_State* L)
	{
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
		int textbox_id = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_background(lua_State* L)
	{
		auto hex = lua_tostring(L, -1);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_foreground(lua_State* L)
	{
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
		int mode = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetWindowMode(static_cast<XenWindowMode>(mode));
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_position(lua_State* L)
	{
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetPosition(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_origin(lua_State* L)
	{
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetOrigin(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_rotation(lua_State* L)
	{
		float angle = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetRotation(angle);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_scale(lua_State* L)
	{
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetScale(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_shader(lua_State* L)
	{
		int asset_id = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->UseShader(asset_id);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_alpha(lua_State* L)
	{
		float alpha = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetAlpha(alpha);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_blend(lua_State* L) 
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		const bool enable = lua_toboolean(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlend(enable);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_blend_func(lua_State* L) 
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const int src = lua_tointeger(L, -2);
		const int dest = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlendFunc(src, dest);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_blend_preset(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "expecting exactly 2 arguments");
		}
		const int preset_num = lua_tointeger(L, -2);
		const BlendPreset preset = static_cast<BlendPreset>(preset_num);
		const bool p_alpha = lua_toboolean(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlendPreset(preset, p_alpha);
		return 0;
	}

	#pragma endregion


	#pragma region Config

	int XentuLuaMachineInterop::config_get_str(lua_State* L)
	{
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

	#pragma endregion


	#pragma region TextBox

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
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxColor(textbox_id, font_id, rr, g, b);
		return 0;
	}

	int XentuLuaMachineInterop::textbox_measure_text(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "expecting exactly 3 arguments");
		}
		auto textbox_id = lua_tointeger(L, -3);
		auto font_id = lua_tointeger(L, -2);
		std::string text = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		auto result = r->MeasureTextBoxText(textbox_id, font_id, text.c_str());
		lua_pushinteger(L, result.x);
		lua_pushinteger(L, result.y);
		return 2;
	}

	#pragma endregion


	#pragma region Input

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

	#pragma endregion


	#pragma region Shader

	int XentuLuaMachineInterop::shader_get_uniform_location(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "expecting exactly 1 arguments");
		}
		auto uniform_name = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		unsigned int loc = r->GetUniformLocation(uniform_name);
		lua_pushinteger(L, loc);
		return 1;
	}

	int XentuLuaMachineInterop::shader_set_uniforms_bool(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "expecting between 2 & 17 arguments");
		int uniform_id = lua_tointeger(L, -argc);
		argc--;
		bool inputs[16];

		for (int i=-argc, j=0; i<0; i++, j++) {
			inputs[j] = lua_toboolean(L, i);
		}

		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}
	
	int XentuLuaMachineInterop::shader_set_uniforms_int(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "expecting between 2 & 17 arguments");
		int uniform_id = lua_tointeger(L, -argc);
		argc--;
		int inputs[16];

		for (int i=-argc, j=0; i<0; i++, j++) {
			inputs[j] = lua_tointeger(L, i);
		}

		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}
	
	int XentuLuaMachineInterop::shader_set_uniforms_float(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "expecting between 2 & 17 arguments");
		int uniform_id = lua_tointeger(L, -argc);
		argc--;
		float inputs[16];

		for (int i=-argc, j=0; i<0; i++, j++) {
			inputs[j] = lua_tonumber(L, i);
		}

		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}

	#pragma endregion


	const char xen::XentuLuaMachineInterop::className[] = "XentuLuaMachineInterop";

	const Luna<XentuLuaMachineInterop>::PropertyType XentuLuaMachineInterop::properties[] = {
		{0,0}
	};

	const Luna<XentuLuaMachineInterop>::FunctionType XentuLuaMachineInterop::methods[] = {
		method(XentuLuaMachineInterop, game_create_window, game_create_window),
		method(XentuLuaMachineInterop, game_on, game_on),
		method(XentuLuaMachineInterop, game_trigger, game_trigger),
		method(XentuLuaMachineInterop, game_run, game_run),
		method(XentuLuaMachineInterop, game_exit, game_exit),
		method(XentuLuaMachineInterop, assets_mount, assets_mount),
		method(XentuLuaMachineInterop, assets_read_text_file, assets_read_text_file),
		method(XentuLuaMachineInterop, assets_load_texture, assets_load_texture),
		method(XentuLuaMachineInterop, assets_load_font, assets_load_font),
		method(XentuLuaMachineInterop, assets_load_sound, assets_load_sound),
		method(XentuLuaMachineInterop, assets_load_music, assets_load_music),
		method(XentuLuaMachineInterop, assets_load_shader, assets_load_shader),
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
		method(XentuLuaMachineInterop, renderer_set_shader, renderer_set_shader),
		method(XentuLuaMachineInterop, renderer_set_alpha, renderer_set_alpha),
		method(XentuLuaMachineInterop, renderer_set_blend, renderer_set_blend),
		method(XentuLuaMachineInterop, renderer_set_blend_func, renderer_set_blend_func),
		method(XentuLuaMachineInterop, renderer_set_blend_preset, renderer_set_blend_preset),
		method(XentuLuaMachineInterop, config_get_str, config_get_str),
		method(XentuLuaMachineInterop, config_get_str2, config_get_str2),
		method(XentuLuaMachineInterop, config_get_bool, config_get_bool),
		method(XentuLuaMachineInterop, config_get_bool2, config_get_bool2),
		method(XentuLuaMachineInterop, config_get_int, config_get_int),
		method(XentuLuaMachineInterop, config_get_int2, config_get_int2),
		method(XentuLuaMachineInterop, textbox_set_text, textbox_set_text),
		method(XentuLuaMachineInterop, textbox_set_color, textbox_set_color),
		method(XentuLuaMachineInterop, textbox_measure_text, textbox_measure_text),
		method(XentuLuaMachineInterop, keyboard_key_down, keyboard_key_down),
		method(XentuLuaMachineInterop, keyboard_key_clicked, keyboard_key_clicked),
		method(XentuLuaMachineInterop, shader_get_uniform_location, shader_get_uniform_location),
		method(XentuLuaMachineInterop, shader_set_uniforms_bool, shader_set_uniforms_bool),
		method(XentuLuaMachineInterop, shader_set_uniforms_int, shader_set_uniforms_int),
		method(XentuLuaMachineInterop, shader_set_uniforms_float, shader_set_uniforms_float),
		{0,0}
	};
}