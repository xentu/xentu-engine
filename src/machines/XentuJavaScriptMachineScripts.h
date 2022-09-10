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

	const renderer = {}
	renderer.clear = renderer_clear;
	renderer.draw_texture = renderer_draw_texture;
	renderer.set_clear_color = function(hex) {
		renderer_set_clear_color(hex.replace('#', ''));
	};

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


)"; }
#endif