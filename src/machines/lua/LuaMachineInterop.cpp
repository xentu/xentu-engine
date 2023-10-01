#include <luna/luna.hpp>

#include "../../Globals.h"
#include "../../Machine.h"
#include "../../vfs/XenVirtualFileSystem.h"
#include "../../vfs/XenFileSystem.h"
#include "../../vfs/XenZipFileSystem.h"
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
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, game.on expects 2 arguments (event_name:s, callback).");
		}

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
			return luaL_error(L, "Error, game.trigger expects 1 argument (event_name:s).");
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
			return luaL_error(L, "Error, assets.mount expects 2 arguments (mount_point:s, fs_path:s).");
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
			return luaL_error(L, "Error, assets.read_text_file expects 1 argument (path:s).");
		}
		auto path = lua_tostring(L, -1);
		auto res = vfs_get_global()->ReadAllText(path);
		lua_pushstring(L, res.c_str());
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_texture(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.load_texture expects 1 argument (path:s).");
		}
		auto path = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		int texture_id = r->LoadTexture(path);
		lua_pushinteger(L, texture_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_font(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, assets.load_font expects 2 arguments (path:s, size:i).");
		}
		const std::string path = lua_tostring(L, -2);
		auto font_size = lua_tointeger(L, -1);
		int font_id = AssetManager::GetInstance()->LoadFont(path, font_size);
		lua_pushinteger(L, font_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_sound(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.load_sound expects 1 argument (path:s).");
		}
		auto path = lua_tostring(L, -1);
		int sound_id = AssetManager::GetInstance()->LoadAudio(path);
		lua_pushinteger(L, sound_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_music(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.load_music expects 1 argument (path:s).");
		}
		auto path = lua_tostring(L, -1);
		int music_id = AssetManager::GetInstance()->LoadMusic(path);
		lua_pushinteger(L, music_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_shader(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, assets.load_shader expects 2 arguments (vertex_code:s, fragment_code:s).");
		}
		auto vertex_src = lua_tostring(L, -2);
		auto frag_src = lua_tostring(L, -1);
		auto assets = AssetManager::GetInstance();
		int asset_id = assets->LoadShader(vertex_src, frag_src);
		lua_pushinteger(L, asset_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_sprite_map(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.load_sprite_map expects 1 argument (path:s).");
		}
		auto path = lua_tostring(L, -1);
		int asset_id = AssetManager::GetInstance()->LoadSpriteMap(path);
		lua_pushinteger(L, asset_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_load_tile_map_tmx(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, assets.load_tile_map_tmx expects 2 arguments (path:s, working_dir:s).");
		}
		auto path = lua_tostring(L, -2);
		auto working_dir = lua_tostring(L, -1);
		int asset_id = AssetManager::GetInstance()->LoadTileMapTMX(path, working_dir);
		lua_pushinteger(L, asset_id);
		return 1;
	}

	int XentuLuaMachineInterop::assets_create_textbox(lua_State* L)
	{
		int count = lua_gettop(L);
		if (count < 2 || count > 3) {
			return luaL_error(L, "Error, assets.create_textbox expects 2 or 3 arguments (w:i, h:i, [wrap:b]).");
		}

		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		auto a = AssetManager::GetInstance();

		if (count < 2 || count > 3) {
			return luaL_error(L, "expecting exactly 2-3 arguments");
		}

		if (count == 2) {
			auto w = lua_tointeger(L, -2);
			auto h = lua_tointeger(L, -1);
			lua_pushinteger(L, a->CreateTextBox(w, h, r->GetForeColor(), true));
			return 1;
		}
		
		auto w = lua_tointeger(L, -3);
		auto h = lua_tointeger(L, -2);
		auto wrap = lua_toboolean(L, -1);
		lua_pushinteger(L, a->CreateTextBox(w, h, r->GetForeColor(), wrap));
		return 1;
	}

	int XentuLuaMachineInterop::assets_unload_texture(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_texture expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadTexture(asset_id));
		return 1;
	}
	
	int XentuLuaMachineInterop::assets_unload_font(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_font expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadFont(asset_id));
		return 1;
	}
	
	int XentuLuaMachineInterop::assets_unload_sound(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_sound expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadAudio(asset_id));
		return 1;
	}
	
	int XentuLuaMachineInterop::assets_unload_music(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_music expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadMusic(asset_id));
		return 1;
	}
	
	int XentuLuaMachineInterop::assets_unload_shader(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_shader expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadShader(asset_id));
		return 1;
	}
	
	int XentuLuaMachineInterop::assets_unload_sprite_map(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_sprite_map expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadSpriteMap(asset_id));
		return 1;
	}

	int XentuLuaMachineInterop::assets_unload_tile_map(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, assets.unload_tile_map expects 1 argument (asset_id:i).");
		}
		auto a = AssetManager::GetInstance();
		int asset_id = lua_tointeger(L, -1);
		lua_pushinteger(L, a->UnloadTileMap(asset_id));
		return 1;
	}

	int XentuLuaMachineInterop::assets_set_wrap(lua_State* L)
	{
		int count = lua_gettop(L);
		if (count == 1) {
			int wrap_both = lua_tointeger(L, -1);
			AssetManager::GetInstance()->SetTextureWrap(wrap_both);
			return 1;
		}
		else if (count == 2) {
			int wrap_s = lua_tointeger(L, -2);
			int wrap_t = lua_tointeger(L, -1);
			AssetManager::GetInstance()->SetTextureWrap(wrap_s, wrap_t);
			return 1;
		}
		return luaL_error(L, "Error, assets.set_wrap expects 1 or 2 arguments ([both:i] min:i, mag:i).");
	}
	
	int XentuLuaMachineInterop::assets_set_interpolation(lua_State* L)
	{
		int count = lua_gettop(L);
		if (count == 1) {
			int both = lua_tointeger(L, -1);
			AssetManager::GetInstance()->SetTextureInterpolation(both);
			return 1;
		}
		else if (count == 2) {
			int min = lua_tointeger(L, -2);
			int mag = lua_tointeger(L, -1);
			AssetManager::GetInstance()->SetTextureInterpolation(min, mag);
			return 1;
		}
		return luaL_error(L, "Error, assets.set_interpolation expects 1 or 2 arguments ([both:i] min:i, mag:i).");
	}

	#pragma endregion


	#pragma region Audio

	int XentuLuaMachineInterop::audio_play_sound(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, audio.play_sound expects 3 arguments (sound_id:i, channel:i, loops:i).");
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
			return luaL_error(L, "Error, audio.play_music expects 2 arguments (music_id:i, loops:i).");
		}
		auto music_id = lua_tointeger(L, -2);
		auto loops = lua_tointeger(L, -1);
		AudioManager::GetInstance()->PlayMusic(music_id, loops);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_stop_sound(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, audio.stop_sound expects 1 argument (channel:i).");
		}
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
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, audio.set_sound_volume expects 2 arguments (sound_id:i, volume:f).");
		}
		auto sound_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetSoundVolume(sound_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_volume(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, audio.set_channel_volume expects 2 arguments (channel:i, volume:f).");
		}
		auto channel_id = lua_tointeger(L, -2);
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetChannelVolume(channel_id, volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_music_volume(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, audio.play_music expects 1 argument (volume:f).");
		}
		float volume = lua_tonumber(L, -1);
		AudioManager::GetInstance()->SetMusicVolume(volume);
		return 0;
	}
	
	int XentuLuaMachineInterop::audio_set_channel_panning(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, audio.set_channel_panning expects 3 arguments (channel:i, left:f, right:f).");
		}
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
		auto reset = lua_gettop(L) > 0 ? lua_toboolean(L, -1) : true;
		auto r = m->GetRenderer();
		r->Begin(reset);
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
			return luaL_error(L, "Error, renderer.draw_texture expects 5 arguments (asset_id:i, x:i, y:i, w:i, h:i).");
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
			return luaL_error(L, "Error, renderer.draw_sub_texture expects 9 arguments (asset_id:i, x:f, y:f, w:f, h:f, sx:f, sy:f, sw:f, sh:f).");
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
			return luaL_error(L, "Error, renderer.draw_rectangle expects 4 arguments (x:f, y:f, w:f, h:f).");
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
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, renderer.draw_textbox expects 3 arguments (asset_id:i, x:f, y:f).");
		}
		int textbox_id = lua_tointeger(L, -3);
		int x = lua_tointeger(L, -2);
		int y = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id, x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_sprite(lua_State* L)
	{
		if (lua_gettop(L) != 7) {
			return luaL_error(L, "Error, renderer.draw_rectangle expects 7 arguments (asset_id:i, group:s, frame:i, x:f, y:f, w:f, h:f).");
		}
		int asset_id = lua_tointeger(L, -7);
		const string region = lua_tostring(L, -6);
		int frame = lua_tointeger(L, -5);
		int x = lua_tointeger(L, -4);
		int y = lua_tointeger(L, -3);
		int w = lua_tointeger(L, -2);
		int h = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSprite(asset_id, region, frame, x, y, w, h);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_draw_tile_layer(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, renderer.draw_tile_layer expects 2 arguments (asset_id:i, layer:i).");
		}
		int asset_id = lua_tointeger(L, -2);
		int layer = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTileLayer(asset_id, layer);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_background(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_background expects 1 argument (hex:s).");
		}
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
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_foreground expects 1 argument (hex:s).");
		}
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
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_window_mode expects 1 argument (mode:i).");
		}
		int mode = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetWindowMode(static_cast<XenWindowMode>(mode));
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_position(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, renderer.set_position expects 2 arguments (x:f, y:f).");
		}
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetPosition(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_origin(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, renderer.set_origin expects 2 arguments (x:f, y:f).");
		}
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetOrigin(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_rotation(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_rotation expects 1 argument (angle:f)");
		}
		float angle = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetRotation(angle);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_scale(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, renderer.set_scale expects 2 arguments (x:f, y:f).");
		}
		float x = lua_tonumber(L, -2);
		float y = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetScale(x, y);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_shader(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_shader expects 1 argument (asset_id:i).");
		}
		int asset_id = lua_tointeger(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->UseShader(asset_id);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_alpha(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_alpha expects 1 argument (alpha:b).");
		}
		float alpha = lua_tonumber(L, -1);
		auto machine = LuaMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetAlpha(alpha);
		return 0;
	}

	int XentuLuaMachineInterop::renderer_set_blend(lua_State* L) 
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, renderer.set_blend expects 1 argument (blend:b).");
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
			return luaL_error(L, "Error, renderer.set_blend_func expects 2 arguments (src:i, dst:i).");
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
			return luaL_error(L, "Error, renderer.set_blend_preset expects 2 arguments (preset:i, alpha:b).");
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
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, config.get_str expects 3 arguments (group:s, name:s, default:s).");
		}
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
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "Error, config.get_str2 expects 4 arguments (group:s, sub_group:s, name:s, default:s).");
		}
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
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, config.get_bool expects 3 arguments (group:s, name:s, default:i).");
		}
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
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "Error, config.get_bool2 expects 4 arguments (group:s, sub_group:s, name:s, default:i).");
		}
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
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, config.get_int expects 3 arguments (group:s, name:s, default:i).");
		}
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
		if (lua_gettop(L) != 4) {
			return luaL_error(L, "Error, config.get_int2 expects 4 arguments (group:s, sub_group:s, name:s, default:i).");
		}
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
			return luaL_error(L, "Error, textbox.set_text expects 3 arguments (textbox_id:i, font_id:i, text:s).");
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
			return luaL_error(L, "Error, textbox.set_color expects 3 arguments (textbox_id:i, font_id:i, hex:s).");
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
			return luaL_error(L, "Error, textbox.measure_text expects 3 arguments (textbox_id:i, font_id:i, text:s).");
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
			return luaL_error(L, "Error, keyboard.key_down expects 1 argument (key_id:i).");
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
			return luaL_error(L, "Error, keyboard.key_clicked expects 1 argument (key_id:i).");
		}
		auto key_code = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyUp(key_code);
		lua_pushboolean(L, down);
		return 1;
	}

	int XentuLuaMachineInterop::mouse_get_position(lua_State* L)
	{
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		auto s = i->GetMouseState();
		lua_pushinteger(L, s->m_x);
		lua_pushinteger(L, s->m_y);
		return 2;
	}

	int XentuLuaMachineInterop::mouse_button_down(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, mouse.button_down expects 1 argument (button_id:i).");
		}
		auto button_code = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->MouseButtonDown(button_code);
		lua_pushboolean(L, down);
		return 1;
	}

	int XentuLuaMachineInterop::mouse_button_clicked(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, mouse.button_clicked expects 1 argument (button_id:i).");
		}
		auto button_code = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->MouseButtonUp(button_code);
		lua_pushboolean(L, down);
		return 1;
	}

	#pragma endregion


	#pragma region Gamepad

	int XentuLuaMachineInterop::gamepad_get_axis(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, mouse.get_axis expects 1 argument (gamepad_id:i).");
		}

		auto index = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		auto s = i->GetGamepadState(index);
		lua_pushinteger(L, s->GetAxisX());
		lua_pushinteger(L, s->GetAxisY());
		return 2;
	}

	int XentuLuaMachineInterop::gamepad_button_down(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, gamepad.button_down expects 2 arguments (gamepad_id:i, button_id:i).");
		}
		auto index = lua_tointeger(L, -2);
		auto button_index = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		auto s = i->GetGamepadState(index);
		bool down = s->IsButtonDown(button_index);
		lua_pushboolean(L, down);
		return 1;
	}

	int XentuLuaMachineInterop::gamepad_button_clicked(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, gamepad.button_clicked expects 2 arguments (gamepad_id:i, button_id:i).");
		}
		auto index = lua_tointeger(L, -2);
		auto button_index = lua_tointeger(L, -1);
		auto m = LuaMachine::GetInstance();
		auto i = m->GetInput();
		auto s = i->GetGamepadState(index);
		bool up = s->IsButtonUp(button_index);
		lua_pushboolean(L, up);
		return 1;
	}

	#pragma endregion


	#pragma region Shader

	int XentuLuaMachineInterop::shader_get_location(lua_State* L)
	{
		if (lua_gettop(L) != 1) {
			return luaL_error(L, "Error, shader.get_location expects 1 argument (uniform:s).");
		}
		auto uniform_name = lua_tostring(L, -1);
		auto m = LuaMachine::GetInstance();
		auto r = m->GetRenderer();
		unsigned int loc = r->GetUniformLocation(uniform_name);
		lua_pushinteger(L, loc);
		return 1;
	}

	int XentuLuaMachineInterop::shader_set_bool(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "Error, shader.set_bool expects 1 uniform id, and upto 16 optional arguments (uniform:s, [values...]).");
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
	
	int XentuLuaMachineInterop::shader_set_int(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "Error, shader.set_int expects 1 uniform name, and upto 16 optional arguments (uniform:s, [values...]).");
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
	
	int XentuLuaMachineInterop::shader_set_float(lua_State* L)
	{
		int argc = lua_gettop(L);
		if (argc < 2 || argc > 17) return luaL_error(L, "Error, shader.set_float expects 1 uniform name, and upto 16 optional arguments (uniform_id:s, [values...]).");
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


	#pragma region Sprite Map

	int XentuLuaMachineInterop::sprite_map_get_frame_info(lua_State* L)
	{
		if (lua_gettop(L) != 3) {
			return luaL_error(L, "Error, sprite_map.get_frame_info expects 3 arguments (asset_id:i, group:s, frame:i).");
		}
		int asset_id = lua_tointeger(L, -3);
		string animation = lua_tostring(L, -2);
		int frame = lua_tointeger(L, -1);
		auto a = AssetManager::GetInstance();
		auto sm = a->GetSpriteMap(asset_id);
		auto grp = sm->get_group(animation);
		auto info = grp->get_frame(frame);

		lua_pushinteger(L, info->delay);
		lua_pushboolean(L, info->flip_x);
		lua_pushboolean(L, info->flip_y);
		return 3;
	}

	int XentuLuaMachineInterop::sprite_map_get_frame_count(lua_State* L)
	{
		if (lua_gettop(L) != 2) {
			return luaL_error(L, "Error, sprite_map.get_frame_count expects 2 arguments (asset_id:i, group:s).");
		}
		int asset_id = lua_tointeger(L, -2);
		string animation = lua_tostring(L, -1);

		auto a = AssetManager::GetInstance();
		auto sm = a->GetSpriteMap(asset_id);
		auto grp = sm->get_group(animation);
		const int count = grp->get_count();
		lua_pushinteger(L, count);
		return 1;
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
		method(XentuLuaMachineInterop, assets_load_sprite_map, assets_load_sprite_map),
		method(XentuLuaMachineInterop, assets_load_tile_map_tmx, assets_load_tile_map_tmx),
		method(XentuLuaMachineInterop, assets_create_textbox, assets_create_textbox),
		method(XentuLuaMachineInterop, assets_unload_texture, assets_unload_texture),
		method(XentuLuaMachineInterop, assets_unload_font, assets_unload_font),
		method(XentuLuaMachineInterop, assets_unload_sound, assets_unload_sound),
		method(XentuLuaMachineInterop, assets_unload_music, assets_unload_music),
		method(XentuLuaMachineInterop, assets_unload_shader, assets_unload_shader),
		method(XentuLuaMachineInterop, assets_unload_sprite_map, assets_unload_sprite_map),
		method(XentuLuaMachineInterop, assets_unload_tile_map, assets_unload_tile_map),
		method(XentuLuaMachineInterop, assets_set_wrap, assets_set_wrap),
		method(XentuLuaMachineInterop, assets_set_interpolation, assets_set_interpolation),
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
		method(XentuLuaMachineInterop, renderer_draw_sprite, renderer_draw_sprite),
		method(XentuLuaMachineInterop, renderer_draw_tile_layer, renderer_draw_tile_layer),
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
		method(XentuLuaMachineInterop, mouse_get_position, mouse_get_position),
		method(XentuLuaMachineInterop, mouse_button_down, mouse_button_down),
		method(XentuLuaMachineInterop, mouse_button_clicked, mouse_button_clicked),
		method(XentuLuaMachineInterop, gamepad_get_axis, gamepad_get_axis),
		method(XentuLuaMachineInterop, gamepad_button_down, gamepad_button_down),
		method(XentuLuaMachineInterop, gamepad_button_clicked, gamepad_button_clicked),
		method(XentuLuaMachineInterop, shader_get_location, shader_get_location),
		method(XentuLuaMachineInterop, shader_set_bool, shader_set_bool),
		method(XentuLuaMachineInterop, shader_set_int, shader_set_int),
		method(XentuLuaMachineInterop, shader_set_float, shader_set_float),
		method(XentuLuaMachineInterop, sprite_map_get_frame_info, sprite_map_get_frame_info),
		method(XentuLuaMachineInterop, sprite_map_get_frame_count, sprite_map_get_frame_count),
		{0,0}
	};
}