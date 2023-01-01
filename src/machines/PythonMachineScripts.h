#pragma once
/**
 * This file holds the python code to be executed when a python engine is loaded
 * by the user. The file does the following:
 * 
 * - Inject an extra loader into importlib that uses the engines vfs.
 * - Add py exclusive modules (const, geometry) under the xentu dummy module.
 * - Add aliases for c++ modules like vfs under the xentu dummy module.
 */
namespace xen { const char * xen_py_script_init = R"(
import sys, importlib.util, xentu
from types import ModuleType


# define a dummy class for our psudo modules.
class XenDummyObject(ModuleType):
	pass


# masquerade a new module for game
game = XenDummyObject('game')
game.create_window = xentu.game_create_window
game.run = xentu.game_run
game.on = xentu.game_on
game.trigger = xentu.game_trigger
game.run = xentu.game_run
game.exit = xentu.game_exit

# masquerade a new module for assets
assets = XenDummyObject('game')
assets.mount = xentu.assets_mount
assets.read_text_file = xentu.assets_read_text_file
assets.load_texture = xentu.assets_load_texture
assets.load_font = xentu.assets_load_font
assets.load_sound = xentu.assets_load_sound
assets.load_music = xentu.assets_load_music
assets.load_shader = xentu.assets_load_shader
assets.load_sprite_map = xentu.assets_load_sprite_map
assets.load_tile_map_tmx = xentu.assets_load_tile_map_tmx
assets.create_textbox = xentu.assets_create_textbox
assets.unload_texture = xentu.assets_unload_texture
assets.unload_font = xentu.assets_unload_font
assets.unload_sound = xentu.assets_unload_sound
assets.unload_music = xentu.assets_unload_music
assets.unload_shader = xentu.assets_unload_shader
assets.unload_sprite_map = xentu.assets_unload_sprite_map
assets.unload_tile_map = xentu.assets_unload_tile_map

def include(path):
	code = xentu.assets_read_text_file(path)
	eval(code)

# masquerade a new module for audio
audio = XenDummyObject('audio')
audio.play_sound = xentu.audio_play_sound
audio.play_music = xentu.audio_play_music
audio.stop_sound = xentu.audio_stop_sound
audio.stop_music = xentu.audio_stop_music
audio.set_sound_volume = xentu.audio_set_sound_volume
audio.set_channel_volume = xentu.audio_set_channel_volume
audio.set_music_volume = xentu.audio_set_music_volume
audio.set_channel_panning = xentu.audio_set_channel_panning


# masquerade a new module for config
config = XenDummyObject('config')
config.get_str = xentu.config_get_str
config.get_str2 = xentu.config_get_str2
config.get_bool = xentu.config_get_bool
config.get_bool2 = xentu.config_get_bool2
config.get_int = xentu.config_get_int
config.get_int2 = xentu.config_get_int2


# new module for renderer
class XenRendererObject(ModuleType):
	def set_background(self, hex):
		xentu.renderer_set_background(hex.replace('#', ''))
	def set_foreground(self, hex):
		xentu.renderer_set_foreground(hex.replace('#', ''))

renderer = XenRendererObject('renderer')
renderer.begin = xentu.renderer_begin
renderer.clear = xentu.renderer_clear
renderer.present = xentu.renderer_present
renderer.draw_texture = xentu.renderer_draw_texture
renderer.draw_sub_texture = xentu.renderer_draw_sub_texture
renderer.draw_rectangle = xentu.renderer_draw_rectangle
renderer.draw_textbox = xentu.renderer_draw_textbox
renderer.draw_sprite = xentu.renderer_draw_sprite
renderer.draw_tile_layer = xentu.renderer_draw_tile_layer
renderer.set_window_mode = xentu.renderer_set_window_mode
renderer.set_position = xentu.renderer_set_position
renderer.set_origin = xentu.renderer_set_origin
renderer.set_rotation = xentu.renderer_set_rotation
renderer.set_scale = xentu.renderer_set_scale
renderer.set_shader = xentu.renderer_set_shader
renderer.set_alpha = xentu.renderer_set_alpha
renderer.set_blend = xentu.renderer_set_blend
renderer.set_blend_func = xentu.renderer_set_blend_func
renderer.set_blend_preset = xentu.renderer_set_blend_preset

# new module for textbox
class XenTextBoxObject(ModuleType):
	def set_color(self, textbox_id, font_id, hex):
		xentu.textbox_set_color(textbox_id, font_id, hex.replace('#', ''))

textbox = XenTextBoxObject('textbox')
textbox.set_text = xentu.textbox_set_text
textbox.measure_text = xentu.textbox_measure_text

# new module for keyboard
keyboard = XenDummyObject('keyboard')
keyboard.key_down = xentu.keyboard_key_down
keyboard.key_clicked = xentu.keyboard_key_clicked

# new module for mouse
mouse = XenDummyObject('mouse')
mouse.button_down = xentu.mouse_button_down
mouse.button_clicked = xentu.mouse_button_clicked

# new module for shaders
shader = XenDummyObject('shader')
shader.get_uniform_location = xentu.shader_get_uniform_location
shader.set_uniforms_bool = xentu.shader_set_uniforms_bool
shader.set_uniforms_int = xentu.shader_set_uniforms_int
shader.set_uniforms_float = xentu.shader_set_uniforms_float

# new module for sprite_map
sprite_map = XenDummyObject('sprite_map')
# sprite_map.get_frame_info = sprite_map_get_frame_info
# sprite_map.get_frame_count = sprite_map_get_frame_count

# masquerade a new module for const
const = XenDummyObject('const')
const.KB_SPACE					= 44
const.KB_APOSTROPHE			= 52
const.KB_COMMA					= 54
const.KB_MINUS					= 45
const.KB_PERIOD				= 55
const.KB_SLASH					= 56
const.KB_1						= 30
const.KB_2						= 31
const.KB_3						= 32
const.KB_4						= 33
const.KB_5						= 34
const.KB_6						= 35
const.KB_7						= 36
const.KB_8						= 37
const.KB_9						= 38
const.KB_0						= 39
const.KB_SEMICOLON			= 51
const.KB_EQUAL					= 103
const.KB_A						= 4
const.KB_B						= 5
const.KB_C						= 6
const.KB_D						= 7
const.KB_E						= 8
const.KB_F						= 9
const.KB_G						= 10
const.KB_H						= 11
const.KB_I						= 12
const.KB_J						= 13
const.KB_K						= 14
const.KB_L						= 15
const.KB_M						= 16
const.KB_N						= 17
const.KB_O						= 18
const.KB_P						= 19
const.KB_Q						= 20
const.KB_R						= 21
const.KB_S						= 22
const.KB_T						= 23
const.KB_U						= 24
const.KB_V						= 25
const.KB_W						= 26
const.KB_X						= 27
const.KB_Y						= 28
const.KB_Z						= 29
const.KB_LEFT_BRACKET		= 184
const.KB_BACKSLASH			= 100
const.KB_RIGHT_BRACKET		= 185
const.KB_GRAVE_ACCENT		= 53
const.KB_ESCAPE				= 41
const.KB_ENTER					= 88
const.KB_TAB					= 186
const.KB_BACKSPACE			= 187
const.KB_INSERT				= 73
const.KB_DELETE				= 76
const.KB_RIGHT					= 79
const.KB_LEFT					= 80
const.KB_DOWN					= 81
const.KB_UP						= 82
const.KB_PAGE_UP				= 75
const.KB_PAGE_DOWN			= 78
const.KB_HOME					= 74
const.KB_END					= 77
const.KB_CAPS_LOCK			= 57
const.KB_SCROLL_LOCK			= 71
const.KB_NUM_LOCK				= 83
const.KB_PRINT_SCREEN		= 70
const.KB_PAUSE					= 72
const.KB_F1						= 58
const.KB_F2						= 59
const.KB_F3						= 60
const.KB_F4						= 61
const.KB_F5						= 62
const.KB_F6						= 63
const.KB_F7						= 64
const.KB_F8						= 65
const.KB_F9						= 66
const.KB_F10					= 67
const.KB_F11					= 68
const.KB_F12					= 69
const.KB_F13					= 104
const.KB_F14					= 105
const.KB_F15					= 106
const.KB_F16					= 107
const.KB_F17					= 108
const.KB_F18					= 109
const.KB_F19					= 110
const.KB_F20					= 111
const.KB_F21					= 112
const.KB_F22					= 113
const.KB_F23					= 114
const.KB_F24					= 115
const.KB_KP_1					= 89
const.KB_KP_2					= 90
const.KB_KP_3					= 91
const.KB_KP_4					= 92
const.KB_KP_5					= 93
const.KB_KP_6					= 94
const.KB_KP_7					= 95
const.KB_KP_8					= 96
const.KB_KP_9					= 97
const.KB_KP_0					= 98
const.KB_KP_DECIMAL			= 220
const.KB_KP_DIVIDE			= 84
const.KB_KP_MULTIPLY			= 85
const.KB_KP_SUBTRACT			= 86
const.KB_KP_ADD				= 87
const.KB_KP_ENTER				= 88
const.KB_KP_EQUAL				= 103
const.KB_LEFT_SHIFT			= 225
const.KB_LEFT_CONTROL		= 224
const.KB_LEFT_ALT				= 226
const.KB_LEFT_SUPER			= 227
const.KB_RIGHT_SHIFT			= 229
const.KB_RIGHT_CONTROL		= 228
const.KB_RIGHT_ALT			= 230
const.KB_RIGHT_SUPER			= 231
const.KB_MENU					= 257


globals()['game'] = sys.modules['game'] = game
globals()['assets'] = sys.modules['assets'] = assets
globals()['audio'] = sys.modules['audio'] = audio
globals()['renderer'] = sys.modules['renderer'] = renderer
globals()['config'] = sys.modules['config'] = config
globals()['textbox'] = sys.modules['textbox'] = textbox
globals()['keyboard'] = sys.modules['keyboard'] = keyboard
globals()['mouse'] = sys.modules['mouse'] = mouse
globals()['const'] = sys.modules['const'] = const
globals()['shader'] = sys.modules['shader'] = shader
globals()['sprite_map'] = sys.modules['sprite_map'] = sprite_map

print("hello1")

# replace the default importer with our own.
class XenImporter(object):
	def find_module(self, module_name, package_path): return self
	def load_module(self, module_name):
		print("Locating module: " + module_name)
		_source = xentu.assets_read_text_file("/" + module_name.replace(".py", "") + ".py")
		if (len(_source)):
			_spec = importlib.util.spec_from_loader(module_name, loader=None)
			_module = importlib.util.module_from_spec(_spec)
			exec(_source, _module.__dict__)
			sys.modules[module_name] = _module
			globals()[module_name] = _module
			return __import__(module_name)
		return self
sys.meta_path.append(XenImporter())

print("hello1")

game.create_window()

)"; }