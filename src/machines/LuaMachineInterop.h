#pragma once

namespace xen
{
	class XentuLuaMachineInterop
	{
		public:
			XentuLuaMachineInterop(lua_State* L);
			~XentuLuaMachineInterop();

			int game_create_window(lua_State* L);
			int game_on(lua_State* L);
			int game_trigger(lua_State* L);
			int game_run(lua_State* L);
			int game_exit(lua_State* L);
			
			int assets_mount(lua_State* L);
			int assets_read_text_file(lua_State* L);
			int assets_load_texture(lua_State* L);
			int assets_load_font(lua_State* L);
			int assets_load_sound(lua_State* L);
			int assets_load_music(lua_State* L);
			int assets_load_shader(lua_State* L);
			int assets_load_sprite_map(lua_State* L);
			int assets_load_tile_map_tmx(lua_State* L);
			int assets_create_textbox(lua_State* L);
			int assets_unload_texture(lua_State* L);
			int assets_unload_font(lua_State* L);
			int assets_unload_sound(lua_State* L);
			int assets_unload_music(lua_State* L);
			int assets_unload_shader(lua_State* L);
			int assets_unload_sprite_map(lua_State* L);
			int assets_unload_tile_map(lua_State* L);

			int audio_play_sound(lua_State* L);
			int audio_play_music(lua_State* L);
			int audio_stop_sound(lua_State* L);
			int audio_stop_music(lua_State* L);
			int audio_set_sound_volume(lua_State* L);
			int audio_set_channel_volume(lua_State* L);
			int audio_set_music_volume(lua_State* L);
			int audio_set_channel_panning(lua_State* L);

			int renderer_begin(lua_State* L);
			int renderer_clear(lua_State* L);
			int renderer_present(lua_State* L);
			int renderer_draw_texture(lua_State* L);
			int renderer_draw_sub_texture(lua_State* L);
			int renderer_draw_rectangle(lua_State* L);
			int renderer_draw_textbox(lua_State* L);
			int renderer_draw_sprite(lua_State* L);
			int renderer_draw_tile_layer(lua_State* L);
			int renderer_set_background(lua_State* L);
			int renderer_set_foreground(lua_State* L);
			int renderer_set_window_mode(lua_State* L);
			int renderer_set_position(lua_State* L);
			int renderer_set_origin(lua_State* L);
			int renderer_set_rotation(lua_State* L);
			int renderer_set_scale(lua_State* L);
			int renderer_set_shader(lua_State* L);
			int renderer_set_alpha(lua_State* L);
			int renderer_set_blend(lua_State* L);
			int renderer_set_blend_func(lua_State* L);
			int renderer_set_blend_preset(lua_State* L);

			int config_get_str(lua_State* L);
			int config_get_str2(lua_State* L);
			int config_get_bool(lua_State* L);
			int config_get_bool2(lua_State* L);
			int config_get_int(lua_State* L);
			int config_get_int2(lua_State* L);

			int textbox_set_text(lua_State* L);
			int textbox_set_color(lua_State* L);
			int textbox_measure_text(lua_State* L);

			int keyboard_key_down(lua_State* L);
			int keyboard_key_clicked(lua_State* L);

			int mouse_get_position(lua_State* L);
			int mouse_button_down(lua_State* L);
			int mouse_button_clicked(lua_State* L);

			int shader_get_location(lua_State* L);
			int shader_set_bool(lua_State* L);
			int shader_set_int(lua_State* L);
			int shader_set_float(lua_State* L);

			int sprite_map_get_frame_info(lua_State* L);
			int sprite_map_get_frame_count(lua_State* L);


		public:
			//Class Constants
			static const char className[];

			// List of class properties that one can set/get from Lua
			static const Luna<XentuLuaMachineInterop>::PropertyType properties[];

			// List of class methods to make available in Lua
			static const Luna<XentuLuaMachineInterop>::FunctionType methods[];
	};
}