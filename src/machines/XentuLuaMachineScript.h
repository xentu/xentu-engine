#ifndef XEN_LUA_MACHINE_SCRIPTS_H
#define XEN_LUA_MACHINE_SCRIPTS_H
/**
 * This file holds the lua code to be executed when a lua engine is loaded
 * by the user. The file does the following:
 * 
 */
namespace xen { const char * xen_lua_script_init = R"(

interop = XentuLuaMachineInterop()

geometry = {}
geometry.create_rect = function(x, y, width, height)
	return { x, y, width, height }
end

game = {}
game.test = function() interop.test() end
game.create_window = function() interop.game_create_window() end
game.on = function(a, b) interop.game_on(a, b) end
game.trigger = function(a) interop.game_trigger(a) end
game.run = function() interop.game_run() end
game.exit = function() interop.game_exit() end

assets = {}
assets.mount = function(a, b) interop.assets_mount(a, b) end
assets.read_text_file = function(a) return interop.assets_read_text_file(a) end
assets.load_texture = function(a) return interop.assets_load_texture(a) end
assets.load_font = function(a, b) return interop.assets_load_font(a, b) end
assets.create_textbox = function(a, b, c, d) return interop.assets_create_textbox(a, b, c, d) end

renderer = {}
renderer.begin = function() interop.renderer_begin() end
renderer.present = function() interop.renderer_present() end
renderer.draw_texture = function(a, b, c, d, e) interop.renderer_draw_texture(a, b, c, d, e) end
renderer.draw_sub_texture = function(a, b,c,d,e, f,g,h,i) interop.renderer_draw_sub_texture(a, b,c,d,e, f,g,h,i) end
renderer.draw_textbox = function(a) interop.renderer_draw_textbox(a) end
renderer.set_background = function(a) interop.renderer_set_background(a) end

config = {}
config.get_str = function(a,b,c) return interop.config_get_str(a,b,c) end
config.get_str2 = function(a,b,c,d) return interop.config_get_str2(a,b,c,d) end
config.get_bool = function(a,b,c) return interop.config_get_bool(a,b,c) end
config.get_bool2 = function(a,b,c,d) return interop.config_get_bool2(a,b,c,d) end
config.get_int = function(a,b,c) return interop.config_get_int(a,b,c) end
config.get_int2 = function(a,b,c,d) return interop.config_get_int2(a,b,c,d) end

textbox = {}
textbox.set_text = function(a,b,c) interop.textbox_set_text(a,b,c) end

keyboard = {}
keyboard.key_down = function(a) interop.keyboard_key_down(a) end


)"; }
#endif