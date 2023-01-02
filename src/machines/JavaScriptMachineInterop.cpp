#include <stdio.h>
#include <string>
#include <ducktape/duktape.h>

#include "../Globals.h"
#include "../Machine.h"
#include "../vfs/XenVirtualFileSystem.h"
#include "../vfs/XenFileSystem.h"
#include "../vfs/XenZipFileSystem.h"
#include "JavaScriptMachine.h"


namespace xen
{
	void js_error_handler(void *udata, const char *msg) {
		(void) udata;  /* ignored in this case, silence warning */

		/* Note that 'msg' may be NULL. */
		//fprintf(stderr, "> %s\n", );
		XEN_ERROR("> JS Error - %s", (msg ? msg : "no message"));
		//fflush(stderr);
		abort();
	}


	void js_init_method(duk_context *L, const char* reference, duk_c_function func, duk_idx_t nargs) 
	{
		duk_push_c_function(L, func, nargs);
		duk_put_global_string(L, reference);
	}


	void js_init_interop(duk_context *L) {
		js_init_method(L, "print", js_native_print, 1);
		js_init_method(L, "code_eval", js_native_eval, 1);
		js_init_method(L, "game_create_window", js_game_create_window, 0);
		js_init_method(L, "game_on", js_game_on, 2);
		js_init_method(L, "game_trigger", js_game_trigger, 1);
		js_init_method(L, "game_run", js_game_run, 0);
		js_init_method(L, "game_exit", js_game_exit, 0);
		js_init_method(L, "assets_mount", js_assets_mount, 1);
		js_init_method(L, "assets_read_text_file", js_assets_read_text_file, 1);
		js_init_method(L, "assets_load_texture", js_assets_load_texture, 1);
		js_init_method(L, "assets_load_font", js_assets_load_font, 2);
		js_init_method(L, "assets_load_sound", js_assets_load_sound, 1);
		js_init_method(L, "assets_load_music", js_assets_load_music, 1);
		js_init_method(L, "assets_load_shader", js_assets_load_shader, 2);
		js_init_method(L, "assets_load_sprite_map", js_assets_load_sprite_map, 1);
		js_init_method(L, "assets_load_tile_map_tmx", js_assets_load_tile_map_tmx, 2);
		js_init_method(L, "assets_create_textbox", js_assets_create_textbox, 4);
		js_init_method(L, "assets_unload_texture", js_assets_unload_texture, 1);
		js_init_method(L, "assets_unload_font", js_assets_unload_font, 1);
		js_init_method(L, "assets_unload_sound", js_assets_unload_sound, 1);
		js_init_method(L, "assets_unload_music", js_assets_unload_music, 1);
		js_init_method(L, "assets_unload_shader", js_assets_unload_shader, 1);
		js_init_method(L, "assets_unload_sprite_map", js_assets_unload_sprite_map, 1);
		js_init_method(L, "assets_unload_tile_map", js_assets_unload_tile_map, 1);
		js_init_method(L, "assets_set_wrap", js_assets_set_wrap, DUK_VARARGS);
		js_init_method(L, "assets_set_interpolation", js_assets_set_interpolation, DUK_VARARGS);
		js_init_method(L, "audio_play_sound", js_audio_play_sound, 3);
		js_init_method(L, "audio_play_music", js_audio_play_music, 2);
		js_init_method(L, "audio_stop_music", js_audio_stop_music, 1);
		js_init_method(L, "audio_stop_sound", js_audio_stop_sound, 0);
		js_init_method(L, "audio_set_sound_volume", js_audio_set_sound_volume, 2);
		js_init_method(L, "audio_set_channel_volume", js_audio_set_channel_volume, 2);
		js_init_method(L, "audio_set_music_volume", js_audio_set_music_volume, 2);
		js_init_method(L, "audio_set_channel_panning", js_audio_set_channel_panning, 3);
		js_init_method(L, "renderer_begin", js_renderer_begin, DUK_VARARGS);
		js_init_method(L, "renderer_clear", js_renderer_clear, 0);
		js_init_method(L, "renderer_present", js_renderer_present, 0);
		js_init_method(L, "renderer_draw_texture", js_renderer_draw_texture, 5);
		js_init_method(L, "renderer_draw_sub_texture", js_renderer_draw_sub_texture, 9);
		js_init_method(L, "renderer_draw_rectangle", js_renderer_draw_rectangle, 4);
		js_init_method(L, "renderer_draw_textbox", js_renderer_draw_textbox, 1);
		js_init_method(L, "renderer_draw_sprite", js_renderer_draw_sprite, 7);
		js_init_method(L, "renderer_draw_tile_layer", js_renderer_draw_tile_layer, 2);
		js_init_method(L, "renderer_set_background", js_renderer_set_background, 1);
		js_init_method(L, "renderer_set_foreground", js_renderer_set_foreground, 1);
		js_init_method(L, "renderer_set_window_mode", js_renderer_set_window_mode, 1);
		js_init_method(L, "renderer_set_position", js_renderer_set_position, 2);
		js_init_method(L, "renderer_set_origin", js_renderer_set_origin, 2);
		js_init_method(L, "renderer_set_rotation", js_renderer_set_rotation, 1);
		js_init_method(L, "renderer_set_scale", js_renderer_set_scale, 2);
		js_init_method(L, "renderer_set_shader", js_renderer_set_shader, 2);
		js_init_method(L, "renderer_set_alpha", js_renderer_set_alpha, 1);
		js_init_method(L, "renderer_set_blend", js_renderer_set_blend, 1);
		js_init_method(L, "renderer_set_blend_func", js_renderer_set_blend_func, 2);
		js_init_method(L, "renderer_set_blend_preset", js_renderer_set_blend_preset, 2);
		js_init_method(L, "config_get_str", js_config_get_str, 3);
		js_init_method(L, "config_get_str2", js_config_get_str2, 4);
		js_init_method(L, "config_get_bool", js_config_get_bool, 3);
		js_init_method(L, "config_get_bool2", js_config_get_bool2, 4);
		js_init_method(L, "config_get_int", js_config_get_int, 3);
		js_init_method(L, "config_get_int2", js_config_get_int2, 4);
		js_init_method(L, "textbox_set_text", js_textbox_set_text, 3);
		js_init_method(L, "textbox_set_color", js_textbox_set_color, 3);
		js_init_method(L, "textbox_measure_text", js_textbox_measure_text, 3);
		js_init_method(L, "keyboard_key_down", js_keyboard_key_down, 1);
		js_init_method(L, "keyboard_key_clicked", js_keyboard_key_clicked, 1);
		js_init_method(L, "mouse_get_position", js_mouse_get_position, 0);
		js_init_method(L, "mouse_button_down", js_mouse_button_down, 1);
		js_init_method(L, "mouse_button_clicked", js_mouse_button_clicked, 1);
		js_init_method(L, "shader_get_location", js_shader_get_location, 1);
		js_init_method(L, "shader_set_bool", js_shader_set_bool, DUK_VARARGS);
		js_init_method(L, "shader_set_int", js_shader_set_int, DUK_VARARGS);
		js_init_method(L, "shader_set_float", js_shader_set_float, DUK_VARARGS);
		js_init_method(L, "sprite_map_get_frame_info", js_sprite_map_get_frame_info, 3);
		js_init_method(L, "sprite_map_get_frame_count", js_sprite_map_get_frame_count, 2);
	}


	void js_call_func(duk_context *L, std::string callback)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_int_t rc = duk_pcall(L, 0);
			#if XEN_DEBUG
			if (rc != 0) {
				XEN_ERROR("trigger failed: %s\n", duk_safe_to_string(L, -1));
				exit(124);
			}
			#endif
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, std::string arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_string(L, arg0.c_str());
			duk_int_t rc = duk_pcall(L, 1);
			#if XEN_DEBUG
			if (rc != 0) {
				XEN_ERROR("trigger failed: %s\n", duk_safe_to_string(L, -1));
				exit(124);
			}
			#endif
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, int arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_int(L, arg0);
			duk_int_t rc = rc = duk_pcall(L, 1);
			#if XEN_DEBUG
			if (rc != 0) {
				XEN_ERROR("trigger failed: %s\n", duk_safe_to_string(L, -1));
				exit(124);
			}
			#endif
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, float arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_number(L, arg0);
			duk_int_t rc = rc = duk_pcall(L, 1);
			#if XEN_DEBUG
			if (rc != 0) {
				XEN_ERROR("trigger failed: %s\n", duk_safe_to_string(L, -1));
				exit(124);
			}
			#endif
		}

		duk_pop(L);
	}


	#pragma region Native

	duk_ret_t js_native_print(duk_context *L) {
		XEN_ECHO("%s\n", duk_to_string(L, 0));
		return 0;  /* no return value (= undefined) */
	}

	duk_ret_t js_native_eval(duk_context *L) {
		auto code = duk_to_string(L, 0);
		duk_eval_string(L, code);
		return 0;
	}

	#pragma endregion


	#pragma region Game Object


	duk_ret_t js_game_create_window(duk_context *L) {
		JavaScriptMachine* m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->Init();
		return window_id;
	}
	
	int js_on_iter = 0;
	duk_ret_t js_game_on(duk_context *L) {
		auto event_name = duk_to_string(L, 0);
		
		std::string callback_name = "xen_callback" + std::to_string(js_on_iter);
		js_on_iter++;

		duk_dup(L, 1);
		duk_put_global_string(L, callback_name.c_str());

		auto m = JavaScriptMachine::GetInstance();
		m->On(event_name, callback_name);
		return 0;
	}
	
	duk_ret_t js_game_trigger(duk_context *L) {
		auto event_name = duk_to_string(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		m->Trigger(event_name);
		return 0;
	}
	
	duk_ret_t js_game_run(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		m->Run();
		return 0;
	}

	duk_ret_t js_game_exit(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Exit();
		return 0;
	}


	#pragma endregion


	#pragma region Assets
	
	duk_ret_t js_assets_mount(duk_context *L) {
		auto s_point = duk_to_string(L, 0);
		auto s_path = duk_to_string(L, 1);
		// create the file system mount & init.
		XenFileSystemPtr zip_fs(new XenZipFileSystem(s_path, "/"));
		zip_fs->Initialize();
		// add the file systems to the vfs.
		XenVirtualFileSystemPtr vfs = vfs_get_global();
    	vfs->AddFileSystem(s_point, zip_fs);
		return 0;
	}
	
	duk_ret_t js_assets_read_text_file(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto res = vfs_get_global()->ReadAllText(path);
		duk_push_string(L, res.c_str());
		return 1;
	}
	
	duk_ret_t js_assets_load_texture(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		int texture_id = r->LoadTexture(path);
		duk_push_int(L, texture_id);
		return 1;
	}

	duk_ret_t js_assets_load_font(duk_context *L) {
		auto path = duk_to_string(L, 0);
		int font_size = duk_to_int(L, 1);
		auto m = JavaScriptMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		int font_id = r->LoadFont(path, font_size);
		duk_push_int(L, font_id);
		return 1;
	}

	duk_ret_t js_assets_load_sound(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto assets = AssetManager::GetInstance();
		int sound_id = assets->LoadAudio(path);
		duk_push_int(L, sound_id);
		return 1;
	}

	duk_ret_t js_assets_load_music(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto assets = AssetManager::GetInstance();
		int music_id = assets->LoadMusic(path);
		duk_push_int(L, music_id);
		return 1;
	}

	duk_ret_t js_assets_load_shader(duk_context *L) {
		auto vertex_src = duk_to_string(L, 0);
		auto frag_src = duk_to_string(L, 1);
		auto assets = AssetManager::GetInstance();
		int shader_id = assets->LoadShader(vertex_src, frag_src);
		duk_push_int(L, shader_id);
		return 1;
	}

	duk_ret_t js_assets_load_sprite_map(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto a = AssetManager::GetInstance();
		int asset_id = a->LoadSpriteMap(path);
		duk_push_int(L, asset_id);
		return 1;
	}

	duk_ret_t js_assets_load_tile_map_tmx(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto working_dir = duk_to_string(L, 1);
		auto a = AssetManager::GetInstance();
		int asset_id = a->LoadTileMapTMX(path, working_dir);
		duk_push_int(L, asset_id);
		return 1;
	}
	
	duk_ret_t js_assets_create_textbox(duk_context *L) {
		auto x = duk_to_int(L, 0);
		auto y = duk_to_int(L, 1);
		auto w = duk_to_int(L, 2);
		auto h = duk_to_int(L, 3);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		auto a = AssetManager::GetInstance();
		int textbox_id = a->CreateTextBox(x, y, w, h, r->GetForeColor());
		duk_push_int(L, textbox_id);
		return 1;
	}

	duk_ret_t js_assets_unload_texture(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadTexture(asset_id));
		return 1;
	}

	duk_ret_t js_assets_unload_font(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadFont(asset_id));
		return 1;
	}

	duk_ret_t js_assets_unload_sound(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadAudio(asset_id));
		return 1;
	}

	duk_ret_t js_assets_unload_music(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadTexture(asset_id));
		return 1;
	}

	duk_ret_t js_assets_unload_shader(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadShader(asset_id));
		return 1;
	}

	duk_ret_t js_assets_unload_sprite_map(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadSpriteMap(asset_id));
		return 1;
	}


	duk_ret_t js_assets_unload_tile_map(duk_context* L) {
		auto a = AssetManager::GetInstance();
		int asset_id = duk_to_int(L, 0);
		duk_push_int(L, a->UnloadTileMap(asset_id));
		return 1;
	}


	duk_ret_t js_assets_set_wrap(duk_context* L) {
		int count = duk_get_top(L);
		if (count == 1) {
			int wrap_both = duk_to_int(L, 0);
			AssetManager::GetInstance()->SetTextureWrap(wrap_both);
			return 1;
		}
		else if (count == 2) {
			int wrap_s = duk_to_int(L, 0);
			int wrap_t = duk_to_int(L, 1);
			AssetManager::GetInstance()->SetTextureWrap(wrap_s, wrap_t);
			return 1;
		}
		return 0;
	}


	duk_ret_t js_assets_set_interpolation(duk_context* L) {
		int count = duk_get_top(L);
		if (count == 1) {
			int both = duk_to_int(L, 0);
			AssetManager::GetInstance()->SetTextureInterpolation(both);
			return 1;
		}
		else if (count == 2) {
			int min = duk_to_int(L, 0);
			int mag = duk_to_int(L, 1);
			AssetManager::GetInstance()->SetTextureInterpolation(min, mag);
			return 1;
		}
		return 0;
	}


	#pragma endregion


	#pragma region Audio

	duk_ret_t js_audio_play_sound(duk_context *L) {
		auto sound_id = duk_to_int(L, 0);
		auto channel = duk_to_int(L, 1);
		auto loops = duk_to_int(L, 2);
		AudioManager::GetInstance()->PlaySound(sound_id, channel, loops);
		return 1;
	}

	duk_ret_t js_audio_play_music(duk_context *L) {
		auto music_id = duk_to_int(L, 0);
		auto loops = duk_to_int(L, 1);
		AudioManager::GetInstance()->PlayMusic(music_id, loops);
		return 0;
	}

	duk_ret_t js_audio_stop_sound(duk_context *L) {
		auto channel = duk_to_int(L, 0);
		AudioManager::GetInstance()->StopSound(channel);
		return 0;
	}

	duk_ret_t js_audio_stop_music(duk_context *L) {
		AudioManager::GetInstance()->StopMusic();
		return 0;
	}

	duk_ret_t js_audio_set_sound_volume(duk_context *L) {
		int sound_id = duk_to_int(L, 0);
		float volume = duk_to_number(L, 1);
		AudioManager::GetInstance()->SetSoundVolume(sound_id, volume);
		return 0;
	}

	duk_ret_t js_audio_set_channel_volume(duk_context *L) {
		int channel_id = duk_to_int(L, 0);
		float volume = duk_to_number(L, 1);
		AudioManager::GetInstance()->SetChannelVolume(channel_id, volume);
		return 0;
	}

	duk_ret_t js_audio_set_music_volume(duk_context *L) {
		float volume = duk_to_number(L, 0);
		AudioManager::GetInstance()->SetMusicVolume(volume);
		return 0;
	}

	duk_ret_t js_audio_set_channel_panning(duk_context *L) {
		int channel_id = duk_to_int(L, 0);
		float left = duk_to_number(L, 1);
		float right = duk_to_number(L, 2);
		AudioManager::GetInstance()->SetChannelPanning(channel_id, left, right);
		return 0;
	}

	#pragma endregion


	#pragma region Renderer

	duk_ret_t js_renderer_begin(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		int argc = duk_get_top(L);
		bool reset = argc > 0 ? duk_to_boolean(L, 0) : true;
		auto r = m->GetRenderer();
		r->Begin(reset);
		return 0;
	}

	duk_ret_t js_renderer_clear(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Clear();
		return 0;
	}

	duk_ret_t js_renderer_present(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Present();
		return 0;
	}
	
	duk_ret_t js_renderer_draw_texture(duk_context *L) {
		int texture_id = duk_to_int(L, 0);
		int x = duk_to_int(L, 1);
		int y = duk_to_int(L, 2);
		int w = duk_to_int(L, 3);
		int h = duk_to_int(L, 4);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTexture(texture_id, x, y, w, h);
		return 0;
	}

	duk_ret_t js_renderer_draw_sub_texture(duk_context *L) {
		int texture_id = duk_to_int(L, 0);
		int x = duk_to_int(L, 1);
		int y = duk_to_int(L, 2);
		int w = duk_to_int(L, 3);
		int h = duk_to_int(L, 4);
		int sx = duk_to_int(L, 5);
		int sy = duk_to_int(L, 6);
		int sw = duk_to_int(L, 7);
		int sh = duk_to_int(L, 8);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSubTexture(texture_id, x, y, w, h, sx, sy, sw, sh);
		return 0;
	}

	duk_ret_t js_renderer_draw_rectangle(duk_context *L) {
		int x = duk_to_int(L, 0);
		int y = duk_to_int(L, 1);
		int w = duk_to_int(L, 2);
		int h = duk_to_int(L, 3);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawRectangle(x, y, w, h);
		return 0;
	}

	duk_ret_t js_renderer_draw_textbox(duk_context *L) {
		int textbox_id = duk_to_int(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return 0;
	}

	duk_ret_t js_renderer_draw_sprite(duk_context *L) {
		int asset_id = duk_to_int(L, 0);
		const string group = duk_to_string(L, 1);
		int frame = duk_to_int(L, 2);
		int x = duk_to_int(L, 3);
		int y = duk_to_int(L, 4);
		int w = duk_to_int(L, 5);
		int h = duk_to_int(L, 6);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSprite(asset_id, group, frame, x, y, w, h);
		return 0;
	}

	duk_ret_t js_renderer_draw_tile_layer(duk_context *L) {
		int asset_id = duk_to_int(L, 0);
		int layer = duk_to_int(L, 1);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTileLayer(asset_id, layer);
		return 0;
	}

	duk_ret_t js_renderer_set_background(duk_context *L) {

		auto hex = duk_to_string(L, 0);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return 0;
	}

	duk_ret_t js_renderer_set_foreground(duk_context *L) {
		auto hex = duk_to_string(L, 0);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetForegroundColor(r, g, b);

		return 0;
	}

	duk_ret_t js_renderer_set_window_mode(duk_context *L) {
		int mode = duk_to_int(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetWindowMode(static_cast<XenWindowMode>(mode));

		return 0;
	}

	duk_ret_t js_renderer_set_position(duk_context *L) { 
		float x = duk_to_number(L, 0);
		float y = duk_to_number(L, 1);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetPosition(x, y);
		return 0;
	}

	duk_ret_t js_renderer_set_origin(duk_context *L) { 
		float x = duk_to_number(L, 0);
		float y = duk_to_number(L, 1);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetOrigin(x, y);
		return 0;
	}
	
	duk_ret_t js_renderer_set_rotation(duk_context *L) {
		float angle = duk_to_number(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetRotation(angle);
		return 0;	
	}

	duk_ret_t js_renderer_set_scale(duk_context *L) {
		float x = duk_to_number(L, 0);
		float y = duk_to_number(L, 1);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetScale(x, y);
		return 0;
	}

	duk_ret_t js_renderer_set_shader(duk_context *L) {
		int asset_id = duk_to_int(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->UseShader(asset_id);
		return 0;
	}

	duk_ret_t js_renderer_set_alpha(duk_context *L) {
		float alpha = duk_to_number(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetAlpha(alpha);
		return 0;
	}

	duk_ret_t js_renderer_set_blend(duk_context *L) {
		bool blend = duk_to_boolean(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlend(blend);
		return 0;
	}

	duk_ret_t js_renderer_set_blend_func(duk_context *L) {
		int src_mode = duk_to_int(L, 0);
		int dest_mode = duk_to_int(L, 1);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlendFunc(src_mode, dest_mode);
		return 0;
	}

	duk_ret_t js_renderer_set_blend_preset(duk_context *L) {
		const int preset_num = duk_to_int(L, 0);
		const BlendPreset preset = static_cast<BlendPreset>(preset_num);
		const bool p_alpha = duk_to_boolean(L, 1);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetBlendPreset(preset, p_alpha);
		return 0;
	}

	#pragma endregion


	#pragma region Config

	duk_ret_t js_config_get_str(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = std::string(duk_to_string(L, 2));
		const auto result = config->GetSetting(m_group, m_name, m_default);
		duk_push_string(L, result.c_str());
		return 1;
	}

	duk_ret_t js_config_get_str2(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_sub_group = std::string(duk_to_string(L, 1));
		const auto m_name = std::string(duk_to_string(L, 2));
		const auto m_default = std::string(duk_to_string(L, 3));
		const auto result = config->GetSetting(m_group, m_sub_group, m_name, m_default);
		duk_push_string(L, result.c_str());
		return 1;
	}

	duk_ret_t js_config_get_bool(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = duk_to_boolean(L, 2);
		const auto result = config->GetSettingBool(m_group, m_name, m_default);
		duk_push_boolean(L, result);
		return 1;
	}

	duk_ret_t js_config_get_bool2(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_sub_group = std::string(duk_to_string(L, 1));
		const auto m_name = std::string(duk_to_string(L, 2));
		const auto m_default = duk_to_boolean(L, 3);
		const auto result = config->GetSettingBool(m_group, m_sub_group, m_name, m_default);
		duk_push_boolean(L, result);
		return 1;
	}

	duk_ret_t js_config_get_int(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = duk_to_int(L, 2);
		const auto result = config->GetSettingInt(m_group, m_name, m_default);
		duk_push_int(L, result);
		return 1;
	}

	duk_ret_t js_config_get_int2(duk_context* L) {
		auto machine = JavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_sub_group = std::string(duk_to_string(L, 1));
		const auto m_name = std::string(duk_to_string(L, 2));
		const auto m_default = duk_to_int(L, 3);
		const auto result = config->GetSettingInt(m_group, m_sub_group, m_name, m_default);
		duk_push_int(L, result);
		return 1;
	}

	#pragma endregion


	#pragma region TextBox

	duk_ret_t js_textbox_set_text(duk_context* L) {
		auto textbox_id = duk_to_int(L, 0);
		auto font_id = duk_to_int(L, 1);
		const char* text = duk_to_string(L, 2);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxText(textbox_id, font_id, text);
		return 0;
	}

	duk_ret_t js_textbox_set_color(duk_context* L) {
		auto textbox_id = duk_to_int(L, 0);
		auto font_id = duk_to_int(L, 1);
		auto hex = duk_to_string(L, 2);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetTextBoxColor(textbox_id, font_id, r, g, b);
		return 0;
	}

	duk_ret_t js_textbox_measure_text(duk_context* L) {
		auto textbox_id = duk_to_int(L, 0);
		auto font_id = duk_to_int(L, 1);
		const char* text = duk_to_string(L, 2);
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		auto result = r->MeasureTextBoxText(textbox_id, font_id, text);
		auto obj_id = duk_push_object(L);
		duk_push_string(L, "w");
		duk_push_int(L, result.x);
		duk_put_prop(L, obj_id);
		duk_push_string(L, "h");
		duk_push_int(L, result.y);
		duk_put_prop(L, obj_id);
		return 1;
	}

	#pragma endregion


	#pragma region Input

	duk_ret_t js_keyboard_key_down(duk_context* L) {
		auto key_code = duk_to_int(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyDown(key_code);
		duk_push_boolean(L, down);
		return 1;
	}

	duk_ret_t js_keyboard_key_clicked(duk_context* L) {
		auto key_code = duk_to_int(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->KeyUp(key_code);
		duk_push_boolean(L, down);
		return 1;
	}

	duk_ret_t js_mouse_get_position(duk_context* L) {
		auto m = JavaScriptMachine::GetInstance();
		auto i = m->GetInput();
		auto s = i->GetMouseState();

		duk_idx_t obj = duk_push_object(L);
		duk_push_int(L, s->m_x);
		duk_put_prop_string(L, obj, "x");
		duk_push_int(L, s->m_y);
		duk_put_prop_string(L, obj, "y");
		return 1;
	}

	duk_ret_t js_mouse_button_down(duk_context* L) {
		auto key_code = duk_to_int(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->MouseButtonDown(key_code);
		duk_push_boolean(L, down);
		return 1;
	}

	duk_ret_t js_mouse_button_clicked(duk_context* L) {
		auto key_code = duk_to_int(L, 0);
		auto m = JavaScriptMachine::GetInstance();
		auto i = m->GetInput();
		bool down = i->MouseButtonUp(key_code);
		duk_push_boolean(L, down);
		return 1;
	}

	#pragma endregion


	#pragma region Shader

	duk_ret_t js_shader_get_location(duk_context* L) {
		string uniform_name = duk_to_string(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		unsigned int loc = renderer->GetUniformLocation(uniform_name);
		duk_push_int(L, loc);
		return 1;
	}

	duk_ret_t js_shader_set_bool(duk_context* L) {
		int argc = duk_get_top(L);
		if (argc < 2 || argc > 17) return DUK_RET_TYPE_ERROR;
		argc--; // subtract 1 so argc equals the number of booleans.

		int uniform_id = duk_to_int(L, 0);
		bool inputs[16];

		for (int i=0; i<argc; i++) {
			inputs[i] = duk_to_boolean(L, i + 1);
		}

		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}

	duk_ret_t js_shader_set_int(duk_context* L) {
		int argc = duk_get_top(L);
		if (argc < 2 || argc > 17) return DUK_RET_TYPE_ERROR;
		argc--; // subtract 1 so argc equals the number of booleans.

		int uniform_id = duk_to_int(L, 0);
		int inputs[16];

		for (int i=0; i<argc; i++) {
			inputs[i] = duk_to_int(L, i + 1);
		}

		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}

	duk_ret_t js_shader_set_float(duk_context* L) {
		int argc = duk_get_top(L);
		if (argc < 2 || argc > 17) return DUK_RET_TYPE_ERROR;
		argc--; // subtract 1 so argc equals the number of booleans.

		int uniform_id = duk_to_int(L, 0);
		float inputs[16];

		for (int i=0; i<argc; i++) {
			inputs[i] = duk_to_number(L, i + 1);
		}

		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetUniforms(uniform_id, argc, inputs);
		return 0;
	}

	#pragma endregion


	#pragma region Sprite Map

	duk_ret_t js_sprite_map_get_frame_info(duk_context *L) {
		int asset_id = duk_to_int(L, 0);
		const string animation = duk_to_string(L, 1);
		int frame = duk_to_int(L, 2);
		auto a = AssetManager::GetInstance();
		auto sm = a->GetSpriteMap(asset_id);
		auto grp = sm->get_group(animation);
		auto info = grp->get_frame(frame);

		duk_idx_t obj = duk_push_object(L);
		duk_push_int(L, info->delay);
		duk_put_prop_string(L, obj, "delay");
		duk_push_boolean(L, info->flip_x);
		duk_put_prop_string(L, obj, "flip_x");
		duk_push_boolean(L, info->flip_y);
		duk_put_prop_string(L, obj, "flip_y");

		return 1;
	}

	duk_ret_t js_sprite_map_get_frame_count(duk_context *L) {
		int asset_id = duk_to_int(L, 0);
		const string animation = duk_to_string(L, 1);
		auto a = AssetManager::GetInstance();
		auto sm = a->GetSpriteMap(asset_id);
		auto grp = sm->get_group(animation);
		const int count = grp->get_count();
		duk_push_int(L, count);
		return 1;
	}

	#pragma endregion
}