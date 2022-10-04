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
		fprintf(stderr, "*** FATAL ERROR: %s\n", (msg ? msg : "no message"));
		fflush(stderr);
		abort();
	}


	void js_init_interop(duk_context *L) {
		duk_push_c_function(L, js_native_print, 1 /*nargs*/);
		duk_put_global_string(L, "print");

		duk_push_c_function(L, js_game_create_window, 0 /*nargs*/);
		duk_put_global_string(L, "game_create_window");
      
		duk_push_c_function(L, js_game_on, 2 /*nargs*/);
		duk_put_global_string(L, "game_on");

		duk_push_c_function(L, js_game_trigger, 1 /*nargs*/);
		duk_put_global_string(L, "game_trigger");

		duk_push_c_function(L, js_game_run, 0 /*nargs*/);
		duk_put_global_string(L, "game_run");

		duk_push_c_function(L, js_game_exit, 0 /* nargs */);
		duk_put_global_string(L, "game_exit");

		duk_push_c_function(L, js_geometry_create_rect, 4 /*nargs*/);
		duk_put_global_string(L, "geometry_create_rect");
      
		duk_push_c_function(L, js_assets_mount, 1 /*nargs*/);
		duk_put_global_string(L, "assets_mount");

		duk_push_c_function(L, js_assets_read_text_file, 1 /*nargs*/);
		duk_put_global_string(L, "assets_read_text_file");

		duk_push_c_function(L, js_assets_load_texture, 1 /*nargs*/);
		duk_put_global_string(L, "assets_load_texture");

		duk_push_c_function(L, js_assets_load_font, 2 /*nargs*/);
		duk_put_global_string(L, "assets_load_font");

		duk_push_c_function(L, js_assets_load_sound, 1 /*nargs*/);
		duk_put_global_string(L, "assets_load_sound");

		duk_push_c_function(L, js_assets_load_music, 1 /*nargs*/);
		duk_put_global_string(L, "assets_load_music");

		duk_push_c_function(L, js_assets_load_shader, 2 /*nargs*/);
		duk_put_global_string(L, "assets_load_shader");
      
		duk_push_c_function(L, js_assets_create_textbox, 4 /*nargs*/);
		duk_put_global_string(L, "assets_create_textbox");

		duk_push_c_function(L, js_audio_play_sound, 3 /*nargs*/);
		duk_put_global_string(L, "audio_play_sound");

		duk_push_c_function(L, js_audio_play_music, 2 /*nargs*/);
		duk_put_global_string(L, "audio_play_music");

		duk_push_c_function(L, js_audio_stop_music, 0 /*nargs*/);
		duk_put_global_string(L, "audio_stop_music");

		duk_push_c_function(L, js_audio_stop_sound, 1 /*nargs*/);
		duk_put_global_string(L, "audio_stop_sound");

		duk_push_c_function(L, js_audio_set_sound_volume, 2 /*nargs*/);
		duk_put_global_string(L, "audio_set_sound_volume");

		duk_push_c_function(L, js_audio_set_channel_volume, 2 /*nargs*/);
		duk_put_global_string(L, "audio_set_channel_volume");

		duk_push_c_function(L, js_audio_set_music_volume, 2 /*nargs*/);
		duk_put_global_string(L, "audio_set_music_volume");

		duk_push_c_function(L, js_audio_set_channel_panning, 3 /*nargs*/);
		duk_put_global_string(L, "audio_set_channel_panning");

		duk_push_c_function(L, js_renderer_begin, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_begin");

		duk_push_c_function(L, js_renderer_clear, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_clear");

		duk_push_c_function(L, js_renderer_present, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_present");

		duk_push_c_function(L, js_renderer_draw_texture, 5 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_texture");

		duk_push_c_function(L, js_renderer_draw_sub_texture, 9 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_sub_texture");

		duk_push_c_function(L, js_renderer_draw_rectangle, 4 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_rectangle");

		duk_push_c_function(L, js_renderer_draw_textbox, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_textbox");

		duk_push_c_function(L, js_renderer_set_background, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_background");

		duk_push_c_function(L, js_renderer_set_foreground, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_foreground");

		duk_push_c_function(L, js_renderer_set_window_mode, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_window_mode");

		duk_push_c_function(L, js_renderer_set_position, 2 /*nargs*/);
		duk_put_global_string(L, "renderer_set_position");

		duk_push_c_function(L, js_renderer_set_origin, 2 /*nargs*/);
		duk_put_global_string(L, "renderer_set_origin");

		duk_push_c_function(L, js_renderer_set_rotation, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_rotation");

		duk_push_c_function(L, js_renderer_set_scale, 2 /*nargs*/);
		duk_put_global_string(L, "renderer_set_scale");

		duk_push_c_function(L, js_renderer_set_shader, 2 /*nargs*/);
		duk_put_global_string(L, "renderer_set_shader");

		duk_push_c_function(L, js_renderer_set_alpha, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_alpha");

		duk_push_c_function(L, js_config_get_str, 3 /*nargs*/);
		duk_put_global_string(L, "config_get_str");

		duk_push_c_function(L, js_config_get_str2, 4 /*nargs*/);
		duk_put_global_string(L, "config_get_str2");

		duk_push_c_function(L, js_config_get_bool, 3 /*nargs*/);
		duk_put_global_string(L, "config_get_bool");

		duk_push_c_function(L, js_config_get_bool2, 4 /*nargs*/);
		duk_put_global_string(L, "config_get_bool2");

		duk_push_c_function(L, js_config_get_int, 3 /*nargs*/);
		duk_put_global_string(L, "config_get_int");

		duk_push_c_function(L, js_config_get_int2, 4 /*nargs*/);
		duk_put_global_string(L, "config_get_int2");

		duk_push_c_function(L, js_textbox_set_text, 3 /*nargs*/);
		duk_put_global_string(L, "textbox_set_text");

		duk_push_c_function(L, js_textbox_set_color, 3 /*nargs*/);
		duk_put_global_string(L, "textbox_set_color");

		duk_push_c_function(L, js_textbox_measure_text, 3 /*nargs*/);
		duk_put_global_string(L, "textbox_measure_text");

		duk_push_c_function(L, js_keyboard_key_down, 1 /*nargs*/);
		duk_put_global_string(L, "keyboard_key_down");

		duk_push_c_function(L, js_keyboard_key_clicked, 1 /*nargs*/);
		duk_put_global_string(L, "keyboard_key_clicked");

		duk_push_c_function(L, js_keyboard_key_clicked, 1 /*numargs*/);
		duk_put_global_string(L, "keyboard_key_clicked");

		duk_push_c_function(L, js_shader_get_uniform_location, 1/*numargs*/);
		duk_put_global_string(L, "shader_get_uniform_location");
	}


	void js_call_func(duk_context *L, std::string callback)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_int_t rc = rc = duk_pcall(L, 0);
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, std::string arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_string(L, arg0.c_str());
			duk_int_t rc = rc = duk_pcall(L, 1);
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, int arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_int(L, arg0);
			duk_int_t rc = rc = duk_pcall(L, 1);
		}
		duk_pop(L);
	}


	void js_call_func(duk_context *L, std::string callback, float arg0)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_push_number(L, arg0);
			duk_int_t rc = rc = duk_pcall(L, 1);
		}

		duk_pop(L);
	}


	duk_ret_t js_native_print(duk_context *L) {
		printf("%s\n", duk_to_string(L, 0));
		return 0;  /* no return value (= undefined) */
	}


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

	duk_ret_t js_geometry_create_rect(duk_context *L) {
		return 0;
	}
	
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
		auto res = vfs_get_global()->ReadAllData(path);
		int texture_id = r->LoadTexture(res.buffer, res.length);
		duk_push_int(L, texture_id);
		return 1;
	}

	duk_ret_t js_assets_load_font(duk_context *L) {
		auto path = duk_to_string(L, 0);
		int font_size = duk_to_int(L, 1);
		auto m = JavaScriptMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		duk_push_int(L, font_id);
		return 1;
	}

	duk_ret_t js_assets_load_sound(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int sound_id = assets->LoadAudio(res.buffer, res.length);
		duk_push_int(L, sound_id);
		return 1;
	}

	duk_ret_t js_assets_load_music(duk_context *L) {
		auto path = duk_to_string(L, 0);
		auto assets = AssetManager::GetInstance();
		auto res = vfs_get_global()->ReadAllData(path);
		int music_id = assets->LoadMusic(res.buffer, res.length);
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
	
	duk_ret_t js_assets_create_textbox(duk_context *L) {
		auto x = duk_to_int(L, 0);
		auto y = duk_to_int(L, 1);
		auto w = duk_to_int(L, 2);
		auto h = duk_to_int(L, 3);
		auto m = JavaScriptMachine::GetInstance();
		auto r = AssetManager::GetInstance();
		int textbox_id = r->CreateTextBox(x, y, w, h);
		duk_push_int(L, textbox_id);
		return 1;
	}

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

	duk_ret_t js_renderer_begin(duk_context *L) {
		auto m = JavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Begin();
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

	duk_ret_t js_renderer_set_background(duk_context *L) {

		auto hex = duk_to_string(L, 0);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);

		printf("clear_color: %i,%i,%i (hex %s)\n", r, g, b, hex);

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


	duk_ret_t js_shader_get_uniform_location(duk_context* L) {
		string uniform_name = duk_to_string(L, 0);
		auto machine = JavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		unsigned int loc = renderer->GetUniformLocation(uniform_name);
		duk_push_int(L, loc);
		return 1;
	}
}