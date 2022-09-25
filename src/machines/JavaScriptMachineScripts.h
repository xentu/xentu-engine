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
	game.exit = game_exit;

	const assets = {}
	assets.load_texture = assets_load_texture;
	assets.load_font = assets_load_font;
	assets.create_textbox = assets_create_textbox;

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
	renderer.draw_texture = renderer_draw_texture;
	renderer.draw_sub_texture = renderer_draw_sub_texture;
	renderer.draw_rectangle = renderer_draw_rectangle;
	renderer.set_background = function(hex) {
		renderer_set_background(hex.replace('#', ''));
	};
	renderer.set_foreground = function(hex) {
		renderer_set_foreground(hex.replace('#', ''));
	};
	renderer.set_window_mode = renderer_set_window_mode;
	renderer.present = renderer_present;
	renderer.draw_textbox = renderer_draw_textbox;
	renderer.set_position = renderer_set_position;
	renderer.set_origin = renderer_set_origin;
	renderer.set_rotation = renderer_set_rotation;
	renderer.set_scale = renderer_set_scale;

	const textbox = {}
	textbox.set_text = textbox_set_text;
	textbox.set_color = function(a, b, hex) {
		textbox_set_color(a, b, hex.replace('#', ''));
	};

	const keyboard = {}
	keyboard.key_down = keyboard_key_down;
	keyboard.key_clicked = keyboard_key_clicked;

	const geometry = {
		create_rect: function(x, y, w, h) {
			return { x:x, y:y, width:w, height:h };
		}
	}

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


	game.create_window();
	// vfs.mount('/zip', './assets/test.zip');

)"; }