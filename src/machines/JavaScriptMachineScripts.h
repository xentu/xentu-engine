#pragma once
/**
 * This file holds the javascript code to be executed when a js engine is
 * loaded by the user.
 */
namespace xen { const char * xen_js_script_init = R"(

	const game = {}
	game.create_window = game_create_window;
	game.run = game_run;
	game.on = game_on;
	game.trigger = game_trigger;
	exit = game_exit;

	const assets = {}
	assets.load_texture = assets_load_texture;
	assets.read_text_file = assets_read_text_file;
	assets.load_font = assets_load_font;
	assets.load_sound = assets_load_sound;
	assets.load_music = assets_load_music;
	assets.load_shader = assets_load_shader;
	assets.load_sprite_map = assets_load_sprite_map;
	assets.load_tile_map_tmx = assets_load_tile_map_tmx;
	assets.create_textbox = assets_create_textbox;
	include = function(path) {
		const code = assets_read_text_file(path);
		code_eval(code);
	}
	assets.unload_texture = assets_unload_texture;
	assets.unload_font = assets_unload_font;
	assets.unload_sound = assets_unload_sound;
	assets.unload_music = assets_unload_music;
	assets.unload_shader = assets_unload_shader;
	assets.unload_sprite_map = assets_unload_sprite_map;
	assets.unload_tile_map = assets_unload_tile_map;
	assets.set_wrap = assets_set_wrap;
	assets.set_interpolation = assets_set_interpolation;

	const audio = {}
	audio.play_sound = audio_play_sound;
	audio.play_music = audio_play_music;
	audio.stop_sound = audio_stop_sound;
	audio.stop_music = audio_stop_music;
	audio.set_sound_volume = audio_set_sound_volume;
	audio.set_channel_volume = audio_set_channel_volume;
	audio.set_music_volume = audio_set_music_volume;
	audio.set_channel_panning = audio_set_channel_panning;

	const config = {}
	config.get_str = config_get_str;
	config.get_str2 = config_get_str2;
	config.get_bool = config_get_bool;
	config.get_bool2 = config_get_bool2;
	config.get_int = config_get_int;
	config.get_int2 = config_get_int2;

	const renderer = {}
	renderer.begin = renderer_begin;
	renderer.clear = renderer_clear;
	renderer.present = renderer_present;
	renderer.draw_texture = renderer_draw_texture;
	renderer.draw_sub_texture = renderer_draw_sub_texture;
	renderer.draw_rectangle = renderer_draw_rectangle;
	renderer.draw_textbox = renderer_draw_textbox;
	renderer.draw_sprite = renderer_draw_sprite;
	renderer.draw_tile_layer = renderer_draw_tile_layer;
	renderer.set_background = function(hex) {
		renderer_set_background(hex.replace('#', ''));
	};
	renderer.set_foreground = function(hex) {
		renderer_set_foreground(hex.replace('#', ''));
	};
	renderer.set_window_mode = renderer_set_window_mode;
	renderer.set_position = renderer_set_position;
	renderer.set_origin = renderer_set_origin;
	renderer.set_rotation = renderer_set_rotation;
	renderer.set_scale = renderer_set_scale;
	renderer.set_shader = renderer_set_shader;
	renderer.set_alpha = renderer_set_alpha;
	renderer.set_blend = renderer_set_blend;
	renderer.set_blend_func = renderer_set_blend_func;
	renderer.set_blend_preset = renderer_set_blend_preset;

	const textbox = {}
	textbox.set_text = textbox_set_text;
	textbox.set_color = function(a, b, hex) {
		textbox_set_color(a, b, hex.replace('#', ''));
	};
	textbox.measure_text = textbox_measure_text;

	const keyboard = {}
	keyboard.key_down = keyboard_key_down;
	keyboard.key_clicked = keyboard_key_clicked;

	const mouse = {}
	mouse.get_position = mouse_get_position;
	mouse.button_down = mouse_button_down;
	mouse.button_clicked = mouse_button_clicked;

	const gamepad = {}
	gamepad.get_axis = gamepad_get_axis;

	const shader = {}
	shader.get_location = shader_get_location;
	shader.set_bool = shader_set_bool;
	shader.set_int = shader_set_int;
	shader.set_float = shader_set_float;

	const sprite_map = {}
	sprite_map.get_frame_info = sprite_map_get_frame_info;
	sprite_map.get_frame_count = sprite_map_get_frame_count;

	const color = {
		hex_to_int: function(hex) {
			hex = hex.replace('#', '');
			var bbggrr = hex.substr(4, 2) + hex.substr(2, 2) + hex.substr(0, 2);
    		return parseInt(hex, 16);
		},
		int_to_hex: function(i) {
			var bbggrr =  ("000000" + i.toString(16)).slice(-6);
    		var rrggbb = bbggrr.substr(4, 2) + bbggrr.substr(2, 2) + bbggrr.substr(0, 2);
    		return "#" + rrggbb;
		}
	}


	// keyboard codes ---------------------------------------------------------

	const KB_SPACE					= 44;
	const KB_APOSTROPHE			= 52;
	const KB_COMMA					= 54;
	const KB_MINUS					= 45;
	const KB_PERIOD				= 55;
	const KB_SLASH					= 56;
	const KB_1						= 30;
	const KB_2						= 31;
	const KB_3						= 32;
	const KB_4						= 33;
	const KB_5						= 34;
	const KB_6						= 35;
	const KB_7						= 36;
	const KB_8						= 37;
	const KB_9						= 38;
	const KB_0						= 39;
	const KB_SEMICOLON			= 51;
	const KB_EQUAL					= 103;
	const KB_A						= 4;
	const KB_B						= 5;
	const KB_C						= 6;
	const KB_D						= 7;
	const KB_E						= 8;
	const KB_F						= 9;
	const KB_G						= 10;
	const KB_H						= 11;
	const KB_I						= 12;
	const KB_J						= 13;
	const KB_K						= 14;
	const KB_L						= 15;
	const KB_M						= 16;
	const KB_N						= 17;
	const KB_O						= 18;
	const KB_P						= 19;
	const KB_Q						= 20;
	const KB_R						= 21;
	const KB_S						= 22;
	const KB_T						= 23;
	const KB_U						= 24;
	const KB_V						= 25;
	const KB_W						= 26;
	const KB_X						= 27;
	const KB_Y						= 28;
	const KB_Z						= 29;
	const KB_LEFT_BRACKET		= 184;
	const KB_BACKSLASH			= 100;
	const KB_RIGHT_BRACKET		= 185;
	const KB_GRAVE_ACCENT		= 53;
	//const KB_WORLD_1				= ;
	//const KB_WORLD_2				= ;
	const KB_ESCAPE				= 41;
	const KB_ENTER					= 88;
	const KB_TAB					= 186;
	const KB_BACKSPACE			= 187;
	const KB_INSERT				= 73;
	const KB_DELETE				= 76;
	const KB_RIGHT					= 79;
	const KB_LEFT					= 80;
	const KB_DOWN					= 81;
	const KB_UP						= 82;
	const KB_PAGE_UP				= 75;
	const KB_PAGE_DOWN			= 78;
	const KB_HOME					= 74;
	const KB_END					= 77;
	const KB_CAPS_LOCK			= 57;
	const KB_SCROLL_LOCK			= 71;
	const KB_NUM_LOCK				= 83;
	const KB_PRINT_SCREEN		= 70;
	const KB_PAUSE					= 72;
	const KB_F1						= 58;
	const KB_F2						= 59;
	const KB_F3						= 60;
	const KB_F4						= 61;
	const KB_F5						= 62;
	const KB_F6						= 63;
	const KB_F7						= 64;
	const KB_F8						= 65;
	const KB_F9						= 66;
	const KB_F10					= 67;
	const KB_F11					= 68;
	const KB_F12					= 69;
	const KB_F13					= 104;
	const KB_F14					= 105;
	const KB_F15					= 106;
	const KB_F16					= 107;
	const KB_F17					= 108;
	const KB_F18					= 109;
	const KB_F19					= 110;
	const KB_F20					= 111;
	const KB_F21					= 112;
	const KB_F22					= 113;
	const KB_F23					= 114;
	const KB_F24					= 115;
	//const KB_F25					= ;
	const KB_KP_1					= 89;
	const KB_KP_2					= 90;
	const KB_KP_3					= 91;
	const KB_KP_4					= 92;
	const KB_KP_5					= 93;
	const KB_KP_6					= 94;
	const KB_KP_7					= 95;
	const KB_KP_8					= 96;
	const KB_KP_9					= 97;
	const KB_KP_0					= 98;
	const KB_KP_DECIMAL			= 220;
	const KB_KP_DIVIDE			= 84;
	const KB_KP_MULTIPLY			= 85;
	const KB_KP_SUBTRACT			= 86;
	const KB_KP_ADD				= 87;
	const KB_KP_ENTER				= 88;
	const KB_KP_EQUAL				= 103;
	const KB_LEFT_SHIFT			= 225;
	const KB_LEFT_CONTROL		= 224;
	const KB_LEFT_ALT				= 226;
	const KB_LEFT_SUPER			= 227;
	const KB_RIGHT_SHIFT			= 229;
	const KB_RIGHT_CONTROL		= 228;
	const KB_RIGHT_ALT			= 230;
	const KB_RIGHT_SUPER			= 231;
	const KB_MENU					= 257;


	// mouse codes ------------------------------------------------------------

	const MOUSE_LEFT				= 0;
	const MOUSE_RIGHT				= 1;
	const MOUSE_MIDDLE         = 2;


	// blend factor constants (from opengl) -----------------------------------

	const ZERO 							 = 0;
	const ONE							 = 1;
	const SRC_COLOR					 = 768; //0x0300
	const ONE_MINUS_SRC_COLOR		 = 769; //0x0301
	const DST_COLOR					 = 774; //0x0306
	const ONE_MINUS_DST_COLOR		 = 775; //0x0307
	const SRC_ALPHA					 = 770; //0x0302
	const ONE_MINUS_SRC_ALPHA		 = 771; //0x0303
	const DST_ALPHA					 = 772; //0x0304
	const ONE_MINUS_DST_ALPHA		 = 773; //0x0305
	const CONSTANT_COLOR				 = 32769; //0x8001
	const ONE_MINUS_CONSTANT_COLOR = 32770; //0x8002
	const SRC_ALPHA_SATURATE		 = 776; //0x0308
	const SRC1_COLOR					 = 35065; //0x88F9
	const ONE_MINUS_SRC1_COLOR		 = 35066; //0x88FA
	const SRC1_ALPHA					 = 34185; //0x8589
	const ONE_MINUS_SRC1_ALPHA		 = 35067; //0x88FB


	// blend presets (same as //developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation)

	const BLEND_SOURCE_OVER			 = 0;
	const BLEND_SOURCE_IN			 = 1;
	const BLEND_SOURCE_OUT			 = 2;
	const BLEND_SOURCE_ATOP			 = 3;
	const BLEND_DESTINATION_OVER	 = 4;
	const BLEND_DESTINATION_IN		 = 5;
	const BLEND_DESTINATION_OUT	 = 6;
	const BLEND_DESTINATION_ATOP	 = 7;
	const BLEND_LIGHTER				 = 8;
	const BLEND_COPY					 = 9;
	const BLEND_XOR					 = 10;
	const BLEND_MULTIPLY				 = 11;
	const BLEND_SCREEN				 = 12;
	const BLEND_OVERLAY				 = 13;
	const BLEND_DARKEN				 = 14;
	const BLEND_LIGHTEN				 = 15;
	const BLEND_COLOR_DODGE			 = 16;
	const BLEND_COLOR_BURN			 = 17;
	const BLEND_HARD_LIGHT			 = 18;
	const BLEND_SOFT_LIGHT			 = 19;
	const BLEND_DIFFERENCE			 = 20;
	const BLEND_EXCLUSION			 = 21;
	const BLEND_HUE					 = 22;
	const BLEND_SATURATION			 = 23;
	const BLEND_COLOR					 = 24;
	const BLEND_LUMINOSITY			 = 25;


	const TEX_LINEAR					 = 0;
	const TEX_NEAREST					 = 1;
	const TEX_CLAMP_TO_EDGE			 = 2;
	const TEX_CLAMP_TO_BORDER		 = 3;
	const TEX_MIRRORED_REPEAT		 = 4;
	const TEX_REPEAT					 = 5;


	// stepper feature --------------------------------------------------------


	function Stepper(ms, iv) { this.delay = ms; this.iter = 0; this.value = iv; }
	Stepper.prototype.next = function(dt) {
		const self = this;
		self.iter += dt;
		if (self.iter >= self.delay) {
			self.iter -= self.delay;
			return true;
		}
		return false;
	}
	assets.create_stepper = function(delay, iv) { return new Stepper(delay, iv); }


	// init code --------------------------------------------------------------


	game.create_window();
	// assets.mount('/zip', './assets/test.zip');

)"; }