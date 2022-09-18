#ifndef XEN_JS_MACHINE_INTEROP_CPP
#define XEN_JS_MACHINE_INTEROP_CPP

#include <stdio.h>
#include <string>
#include "XentuJavaScriptMachine.h"

#include "src/fs/XenVirtualFileSystem.h"
#include "src/fs/XenFileSystem.h"
#include "src/fs/XenZipFileSystem.h"

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
      
		duk_push_c_function(L, js_assets_create_textbox, 4 /*nargs*/);
		duk_put_global_string(L, "assets_create_textbox");

		duk_push_c_function(L, js_renderer_begin, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_begin");

		duk_push_c_function(L, js_renderer_present, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_present");

		duk_push_c_function(L, js_renderer_draw_texture, 5 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_texture");

		duk_push_c_function(L, js_renderer_draw_sub_texture, 9 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_sub_texture");

		duk_push_c_function(L, js_renderer_draw_textbox, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_textbox");

		duk_push_c_function(L, js_renderer_set_background, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_background");

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

		duk_push_c_function(L, js_keyboard_key_down, 1 /*nargs*/);
		duk_put_global_string(L, "keyboard_key_down");
	}


	void js_call_func(duk_context *L, std::string callback)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_int_t rc = rc = duk_pcall(L, 0);
		}

		duk_pop(L);
	}


	duk_ret_t js_native_print(duk_context *L) {
		printf("%s\n", duk_to_string(L, 0));
		return 0;  /* no return value (= undefined) */
	}


	duk_ret_t js_game_create_window(duk_context *L) {
		XEN_LOG("- Called game_create_window\n");
		XentuJavaScriptMachine* m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		int window_id = r->CreateWindow();
		return window_id;
	}
	

	int js_on_iter = 0;
	duk_ret_t js_game_on(duk_context *L) {
		XEN_LOG("- Called game_on\n");
		auto event_name = duk_to_string(L, 0);
		
		std::string callback_name = "xen_callback" + std::to_string(js_on_iter);
		js_on_iter++;

		duk_dup(L, 1);
		duk_put_global_string(L, callback_name.c_str());

		auto m = XentuJavaScriptMachine::GetInstance();
		m->On(event_name, callback_name);
		return 0;
	}
	
	duk_ret_t js_game_trigger(duk_context *L) {
		XEN_LOG("- Called game_trigger\n");
		return 0;
	}
	
	duk_ret_t js_game_run(duk_context *L) {
		XEN_LOG("- Called game_run\n");
		auto m = XentuJavaScriptMachine::GetInstance();
		m->Run();
		return 1;
	}

	duk_ret_t js_game_exit(duk_context *L) {
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->Exit();
		return 0;
	}

	duk_ret_t js_geometry_create_rect(duk_context *L) {
		XEN_LOG("- Called geometry_create_rect\n");
		return 0;
	}
	
	duk_ret_t js_assets_mount(duk_context *L) {
		XEN_LOG("- Called assets_mount\n");
		return 0;
	}
	
	duk_ret_t js_assets_read_text_file(duk_context *L) {
		XEN_LOG("- Called assets_read_text_file\n");
		return 0;
	}
	
	duk_ret_t js_assets_load_texture(duk_context *L) {
		XEN_LOG("- Called assets_load_texture\n");
		auto path = duk_to_string(L, 0);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->LoadTexture(res.buffer, res.length);
		duk_push_int(L, texture_id);
		return 1;
	}

	duk_ret_t js_assets_load_font(duk_context *L) {
		XEN_LOG("- Called assets_load_font.\n");
		auto path = duk_to_string(L, 0);
		XEN_LOG("- Attempting to read font %s\n", path);
		int font_size = duk_to_int(L, 1);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		auto res = vfs_get_global()->ReadAllData(path);
		XEN_LOG("- Font bytes read: %s\n", std::to_string(res.length).c_str());
		int font_id = r->LoadFont(res.buffer, res.length, font_size);
		duk_push_int(L, font_id);
		return 1;
	}
	
	duk_ret_t js_assets_create_textbox(duk_context *L) {
		XEN_LOG("- Called assets_load_font.\n");
		auto x = duk_to_int(L, 0);
		auto y = duk_to_int(L, 1);
		auto w = duk_to_int(L, 2);
		auto h = duk_to_int(L, 3);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		int textbox_id = r->CreateTextBox(x, y, w, h);
		duk_push_int(L, textbox_id);
		return 1;
	}

	duk_ret_t js_renderer_begin(duk_context *L) {
		XEN_LOG("- Called renderer_begin\n");
		return 0;
	}

	duk_ret_t js_renderer_present(duk_context *L) {
		XEN_LOG("- Called renderer_present\n");
		return 0;
	}
	
	duk_ret_t js_renderer_draw_texture(duk_context *L) {
		XEN_LOG("- Called renderer_draw_texture\n");
		int texture_id = duk_to_int(L, 0);
		int x = duk_to_int(L, 1);
		int y = duk_to_int(L, 2);
		int w = duk_to_int(L, 3);
		int h = duk_to_int(L, 4);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTexture(texture_id, x, y, w, h);
		return 0;
	}

	duk_ret_t js_renderer_draw_sub_texture(duk_context *L) {
		XEN_LOG("- Called renderer_draw_texture\n");
		int texture_id = duk_to_int(L, 0);
		int x = duk_to_int(L, 1);
		int y = duk_to_int(L, 2);
		int w = duk_to_int(L, 3);
		int h = duk_to_int(L, 4);
		int sx = duk_to_int(L, 5);
		int sy = duk_to_int(L, 6);
		int sw = duk_to_int(L, 7);
		int sh = duk_to_int(L, 8);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawSubTexture(texture_id, x, y, w, h, sx, sy, sw, sh);
		return 0;
	}

	duk_ret_t js_renderer_draw_textbox(duk_context *L) {
		XEN_LOG("- Called renderer_draw_texbox\n");
		int textbox_id = duk_to_int(L, 0);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->DrawTextBox(textbox_id);
		return 0;
	}

	duk_ret_t js_renderer_set_background(duk_context *L) {
		XEN_LOG("- Called renderer_set_background\n");

		auto hex = duk_to_string(L, 0);
		int r, g, b;
		sscanf(hex, "%02x%02x%02x", &r, &g, &b);

		printf("clear_color: 0x%x, 0x%x, 0x%x (hex %s)", r, g, b, hex);

		auto machine = XentuJavaScriptMachine::GetInstance();
		auto renderer = machine->GetRenderer();
		renderer->SetClearColor(r, g, b);

		return 0;
	}

	duk_ret_t js_config_get_str(duk_context* L) {
		auto machine = XentuJavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = std::string(duk_to_string(L, 2));
		const auto result = config->GetSetting(m_group, m_name, m_default);
		duk_push_string(L, result.c_str());
		return 1;
	}

	duk_ret_t js_config_get_str2(duk_context* L) {
		auto machine = XentuJavaScriptMachine::GetInstance();
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
		auto machine = XentuJavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = duk_to_boolean(L, 2);
		const auto result = config->GetSettingBool(m_group, m_name, m_default);
		duk_push_boolean(L, result);
		return 1;
	}

	duk_ret_t js_config_get_bool2(duk_context* L) {
		auto machine = XentuJavaScriptMachine::GetInstance();
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
		auto machine = XentuJavaScriptMachine::GetInstance();
		auto config = machine->GetConfig();
		const auto m_group = std::string(duk_to_string(L, 0));
		const auto m_name = std::string(duk_to_string(L, 1));
		const auto m_default = duk_to_int(L, 2);
		const auto result = config->GetSettingInt(m_group, m_name, m_default);
		duk_push_int(L, result);
		return 1;
	}

	duk_ret_t js_config_get_int2(duk_context* L) {
		auto machine = XentuJavaScriptMachine::GetInstance();
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
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		r->SetTextBoxText(textbox_id, font_id, text);		
		return 0;
	}

	duk_ret_t js_keyboard_key_down(duk_context* L) {
		auto key_code = duk_to_int(L, 0);
		auto m = XentuJavaScriptMachine::GetInstance();
		auto r = m->GetRenderer();
		bool down = r->KeyDown(key_code);
		duk_push_boolean(L, down);
		return 1;
	}
}

#endif