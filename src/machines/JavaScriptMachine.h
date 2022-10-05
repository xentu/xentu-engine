#pragma once

#include <stdio.h>
#include <ducktape/duktape.h>
#include <string>
#include <unordered_map>

namespace xen
{
	class JavaScriptMachine : public Machine
	{
		public:
			JavaScriptMachine(int argc, char *argv[], Config* config);
			~JavaScriptMachine();
			static JavaScriptMachine* GetInstance();
			int Init();
			int Trigger(const string event_name);
			int Trigger(const string event_name, const string arg0);
			int Trigger(const string event_name, const int arg0);
			int Trigger(const string event_name, const float arg0);
			int On(const string event_name, const string callback);
		
		private:
			unordered_multimap<string, string> callbacks;
			duk_context* L;
			static JavaScriptMachine* instance;
	};

	// a error handling callback when something goes wrong in the js engine.
	void js_error_handler(void *udata, const char *msg);

	// called to initialize the native engine functions available to js.
	void js_init_interop(duk_context *L);
	void js_call_func(duk_context *L, string func_name);
	void js_call_func(duk_context *L, string func_name, string arg0);
	void js_call_func(duk_context *L, string func_name, int arg0);
	void js_call_func(duk_context *L, string func_name, float arg0);

	duk_ret_t js_native_print(duk_context* ctx);

	duk_ret_t js_game_create_window(duk_context *L);
	duk_ret_t js_game_on(duk_context *L);
	duk_ret_t js_game_trigger(duk_context *L);
	duk_ret_t js_game_run(duk_context *L);
	duk_ret_t js_game_exit(duk_context *L);

	duk_ret_t js_assets_mount(duk_context *L);
	duk_ret_t js_assets_read_text_file(duk_context *L);
	duk_ret_t js_assets_load_texture(duk_context *L);
	duk_ret_t js_assets_load_font(duk_context *L);
	duk_ret_t js_assets_load_sound(duk_context *L);
	duk_ret_t js_assets_load_music(duk_context *L);
	duk_ret_t js_assets_load_shader(duk_context *L);
	duk_ret_t js_assets_create_textbox(duk_context *L);

	duk_ret_t js_audio_play_sound(duk_context *L);
	duk_ret_t js_audio_play_music(duk_context *L);
	duk_ret_t js_audio_stop_sound(duk_context *L);
	duk_ret_t js_audio_stop_music(duk_context *L);
	duk_ret_t js_audio_set_sound_volume(duk_context *L);
	duk_ret_t js_audio_set_channel_volume(duk_context *L);
	duk_ret_t js_audio_set_music_volume(duk_context *L);
	duk_ret_t js_audio_set_channel_panning(duk_context *L);

	duk_ret_t js_renderer_begin(duk_context *L);
	duk_ret_t js_renderer_clear(duk_context *L);
	duk_ret_t js_renderer_present(duk_context *L);
	duk_ret_t js_renderer_draw_texture(duk_context *L);
	duk_ret_t js_renderer_draw_sub_texture(duk_context *L);
	duk_ret_t js_renderer_draw_rectangle(duk_context *L);
	duk_ret_t js_renderer_draw_textbox(duk_context *L);
	duk_ret_t js_renderer_set_background(duk_context *L);
	duk_ret_t js_renderer_set_foreground(duk_context *L);
	duk_ret_t js_renderer_set_window_mode(duk_context *L);
	duk_ret_t js_renderer_set_position(duk_context *L);
	duk_ret_t js_renderer_set_origin(duk_context *L);
	duk_ret_t js_renderer_set_rotation(duk_context *L);
	duk_ret_t js_renderer_set_scale(duk_context *L);
	duk_ret_t js_renderer_set_shader(duk_context *L);
	duk_ret_t js_renderer_set_alpha(duk_context *L);
	duk_ret_t js_renderer_set_blend(duk_context *L);
	duk_ret_t js_renderer_set_blend_func(duk_context *L);
	
	duk_ret_t js_config_get_str(duk_context* L);
	duk_ret_t js_config_get_str2(duk_context* L);
	duk_ret_t js_config_get_bool(duk_context* L);
	duk_ret_t js_config_get_bool2(duk_context* L);
	duk_ret_t js_config_get_int(duk_context* L);
	duk_ret_t js_config_get_int2(duk_context* L);

	duk_ret_t js_textbox_set_text(duk_context* L);
	duk_ret_t js_textbox_measure_text(duk_context* L);
	duk_ret_t js_textbox_set_color(duk_context* L);

	duk_ret_t js_keyboard_key_down(duk_context* L);
	duk_ret_t js_keyboard_key_clicked(duk_context* L);

	duk_ret_t js_shader_get_uniform_location(duk_context* L);
	duk_ret_t js_shader_set_uniforms_bool(duk_context* L);
	duk_ret_t js_shader_set_uniforms_int(duk_context* L);
	duk_ret_t js_shader_set_uniforms_float(duk_context* L);
	duk_ret_t js_shader_set_uniform_matrix(duk_context* L);
}