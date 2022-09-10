#ifndef XEN_JS_MACHINE_INTEROP_CPP
#define XEN_JS_MACHINE_INTEROP_CPP

#include <stdio.h>
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

		duk_push_c_function(L, js_geometry_create_rect, 4 /*nargs*/);
		duk_put_global_string(L, "geometry_create_rect");
      
		duk_push_c_function(L, js_assets_mount, 1 /*nargs*/);
		duk_put_global_string(L, "assets_mount");

		duk_push_c_function(L, js_assets_read_text_file, 1 /*nargs*/);
		duk_put_global_string(L, "assets_read_text_file");

		duk_push_c_function(L, js_assets_load_texture, 1 /*nargs*/);
		duk_put_global_string(L, "assets_load_texture");
      
		duk_push_c_function(L, js_renderer_clear, 0 /*nargs*/);
		duk_put_global_string(L, "renderer_clear");

		duk_push_c_function(L, js_renderer_draw_texture, 5 /*nargs*/);
		duk_put_global_string(L, "renderer_draw_texture");

		duk_push_c_function(L, js_renderer_set_clear_color, 1 /*nargs*/);
		duk_put_global_string(L, "renderer_set_clear_color");
	}


	void js_call_func(duk_context *L, std::string callback)
	{
		bool t = duk_get_global_string(L, callback.c_str());
		if (t) {
			duk_int_t rc = rc = duk_pcall(L, 0);
		}

		duk_pop(L);
		/* auto code = s_code.c_str();
		bool exists = duk_get_global_string(L, code);
		if (exists) {
			duk_push_global_object(L);
			duk_call_method(L, 0nargs); 
		}
		else {
			duk_eval_string_noresult(L, code);
		} */
	}


	duk_ret_t js_native_print(duk_context *L) {
		printf("%s\n", duk_to_string(L, 0));
		return 0;  /* no return value (= undefined) */
	}

	duk_ret_t js_game_create_window(duk_context *L) {
		XEN_LOG("game_create_window was called\n");
		XentuJavaScriptMachine* m = XentuJavaScriptMachine::get_instance();
		auto r = m->get_renderer();
		int window_id = r->create_window();
		return window_id;
	}
	
	int js_on_iter = 0;
	duk_ret_t js_game_on(duk_context *L) {
		XEN_LOG("game_on was called\n");
		auto event_name = duk_to_string(L, 0);
		
		std::string callback_name = "xen_callback" + std::to_string(js_on_iter);
		js_on_iter++;

		duk_dup(L, 1);
		duk_put_global_string(L, callback_name.c_str());

		auto m = XentuJavaScriptMachine::get_instance();
		m->on(event_name, callback_name);
		return 0;
	}
	
	duk_ret_t js_game_trigger(duk_context *L) {
		XEN_LOG("game_trigger was called\n");
		return 0;
	}
	
	duk_ret_t js_game_run(duk_context *L) {
		XEN_LOG("game_run was called\n");
		auto m = XentuJavaScriptMachine::get_instance();
		m->run();
		return 1;
	}

	duk_ret_t js_geometry_create_rect(duk_context *L) {
		XEN_LOG("geometry_create_rect was called\n");
		return 0;
	}
	
	duk_ret_t js_assets_mount(duk_context *L) {
		XEN_LOG("assets_mount was called\n");
		return 0;
	}
	
	duk_ret_t js_assets_read_text_file(duk_context *L) {
		XEN_LOG("assets_read_text_file was called\n");
		return 0;
	}
	
	duk_ret_t js_assets_load_texture(duk_context *L) {
		XEN_LOG("assets_load_texture was called\n");
		auto path = duk_to_string(L, 0);
		auto m = XentuJavaScriptMachine::get_instance();
		auto r = m->get_renderer();
		auto res = vfs_get_global()->ReadAllData(path);
		printf("Bytes read: %s\n", std::to_string(res.length).c_str());
		int texture_id = r->load_texture(res.buffer, res.length);
		duk_push_int(L, texture_id);
		return 1;
	}
	
	duk_ret_t js_renderer_clear(duk_context *L) {
		XEN_LOG("renderer_clear was called\n");
		return 0;
	}
	
	duk_ret_t js_renderer_draw_texture(duk_context *L) {
		XEN_LOG("renderer_draw_texture was called\n");
		int texture_id = duk_to_int(L, 0);
		int x = duk_to_int(L, 1);
		int y = duk_to_int(L, 2);
		int w = duk_to_int(L, 3);
		int h = duk_to_int(L, 4);
		auto m = XentuJavaScriptMachine::get_instance();
		auto r = m->get_renderer();
		r->draw_texture(texture_id, x, y, w, h);
		return 0;
	}
	
	duk_ret_t js_renderer_set_clear_color(duk_context *L) {
		XEN_LOG("renderer_set_clear_color was called\n");
		return 0;
	}
}

#endif