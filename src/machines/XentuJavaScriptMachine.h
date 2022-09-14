#ifndef XEN_JS_MACHINE
#define XEN_JS_MACHINE

#include "../XentuMachine.h"

#include <ducktape/duktape.h>
#include <string>
#include <unordered_map>

namespace xen
{
	class XentuJavaScriptMachine : public XentuMachine
	{
		public:
			XentuJavaScriptMachine(int argc, char *argv[], XentuConfig* config);
			~XentuJavaScriptMachine();
			static XentuJavaScriptMachine* get_instance();
			int init();
			int trigger(const std::string event_name);
			int on(const std::string event_name, const std::string callback);
		
		private:
			std::unordered_multimap<std::string, std::string> callbacks;
			duk_context* L;
			static XentuJavaScriptMachine* instance;
	};

	// a error handling callback when something goes wrong in the js engine.
	void js_error_handler(void *udata, const char *msg);

	// called to initialize the native engine functions available to js.
	void js_init_interop(duk_context *L);
	void js_call_func(duk_context *L, std::string func_name);


	duk_ret_t js_native_print(duk_context* ctx);
	duk_ret_t js_game_create_window(duk_context *L);
	duk_ret_t js_game_on(duk_context *L);
	duk_ret_t js_game_trigger(duk_context *L);
	duk_ret_t js_game_run(duk_context *L);
	duk_ret_t js_geometry_create_rect(duk_context *L);
	duk_ret_t js_assets_mount(duk_context *L);
	duk_ret_t js_assets_read_text_file(duk_context *L);
	duk_ret_t js_assets_load_texture(duk_context *L);
	duk_ret_t js_assets_load_font(duk_context *L);
	duk_ret_t js_renderer_clear(duk_context *L);
	duk_ret_t js_renderer_draw_texture(duk_context *L);
	duk_ret_t js_renderer_set_clear_color(duk_context *L);

	duk_ret_t js_config_get_str(duk_context* L);
	duk_ret_t js_config_get_str2(duk_context* L);
	duk_ret_t js_config_get_bool(duk_context* L);
	duk_ret_t js_config_get_bool2(duk_context* L);
	duk_ret_t js_config_get_int(duk_context* L);
	duk_ret_t js_config_get_int2(duk_context* L);
}

#endif