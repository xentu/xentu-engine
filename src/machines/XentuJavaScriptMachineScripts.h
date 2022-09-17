#ifndef XEN_JS_MACHINE_SCRIPTS_H
#define XEN_JS_MACHINE_SCRIPTS_H
/**
 * This file holds the javascript code to be executed when a js engine is
 * loaded by the user.
 */
namespace xen { const char * xen_js_script_init = R"(

	const game = {}
	game.create_window = game_create_window;
	game.run = game_run;
	game.on = game_on;

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
	renderer.draw_texture = renderer_draw_texture;
	renderer.set_background = function(hex) {
		renderer_set_background(hex.replace('#', ''));
	};
	renderer.present = renderer_present;
	renderer.draw_textbox = renderer_draw_textbox;

	const textbox = {}
	textbox.set_text = textbox_set_text;

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

	game.create_window();

	// vfs.mount('/zip', './assets/test.zip');
)"; }
#endif