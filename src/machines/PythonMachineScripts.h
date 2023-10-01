#pragma once
#if XEN_PY

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
assets.set_wrap = xentu.assets_set_wrap
assets.set_interpolation = xentu.assets_set_interpolation

def include(path):
	code = xentu.assets_read_text_file(path)
	print("Include " + path)
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

# new module for gamepad
gamepad = XenDummyObject('gamepad')
gamepad.get_axis = xentu.gamepad_get_axis
gamepad.button_down = xentu.gamepad_button_down
gamepad.button_clicked = xentu.gamepad_button_clicked

# new module for shaders
shader = XenDummyObject('shader')
shader.get_location = xentu.shader_get_location
shader.set_bool = xentu.shader_set_bool
shader.set_int = xentu.shader_set_int
shader.set_float = xentu.shader_set_float

# new module for sprite_map
sprite_map = XenDummyObject('sprite_map')
sprite_map.get_frame_info = xentu.sprite_map_get_frame_info
sprite_map.get_frame_count = xentu.sprite_map_get_frame_count

# keyboard constants
globals()['KB_SPACE']				= 44
globals()['KB_APOSTROPHE']			= 52
globals()['KB_COMMA']				= 54
globals()['KB_MINUS']				= 45
globals()['KB_PERIOD']				= 55
globals()['KB_SLASH']				= 56
globals()['KB_1']						= 30
globals()['KB_2']						= 31
globals()['KB_3']						= 32
globals()['KB_4']						= 33
globals()['KB_5']						= 34
globals()['KB_6']						= 35
globals()['KB_7']						= 36
globals()['KB_8']						= 37
globals()['KB_9']						= 38
globals()['KB_0']						= 39
globals()['KB_SEMICOLON']			= 51
globals()['KB_EQUAL']				= 103
globals()['KB_A']						= 4
globals()['KB_B']						= 5
globals()['KB_C']						= 6
globals()['KB_D']						= 7
globals()['KB_E']						= 8
globals()['KB_F']						= 9
globals()['KB_G']						= 10
globals()['KB_H']						= 11
globals()['KB_I']						= 12
globals()['KB_J']						= 13
globals()['KB_K']						= 14
globals()['KB_L']						= 15
globals()['KB_M']						= 16
globals()['KB_N']						= 17
globals()['KB_O']						= 18
globals()['KB_P']						= 19
globals()['KB_Q']						= 20
globals()['KB_R']						= 21
globals()['KB_S']						= 22
globals()['KB_T']						= 23
globals()['KB_U']						= 24
globals()['KB_V']						= 25
globals()['KB_W']						= 26
globals()['KB_X']						= 27
globals()['KB_Y']						= 28
globals()['KB_Z']						= 29
globals()['KB_LEFT_BRACKET']		= 184
globals()['KB_BACKSLASH']			= 100
globals()['KB_RIGHT_BRACKET']		= 185
globals()['KB_GRAVE_ACCENT']		= 53
globals()['KB_ESCAPE']				= 41
globals()['KB_ENTER']				= 88
globals()['KB_RETURN']           = 40
globals()['KB_TAB']					= 186
globals()['KB_BACKSPACE']			= 187
globals()['KB_INSERT']				= 73
globals()['KB_DELETE']				= 76
globals()['KB_RIGHT']				= 79
globals()['KB_LEFT']					= 80
globals()['KB_DOWN']					= 81
globals()['KB_UP']					= 82
globals()['KB_PAGE_UP']				= 75
globals()['KB_PAGE_DOWN']			= 78
globals()['KB_HOME']					= 74
globals()['KB_END']					= 77
globals()['KB_CAPS_LOCK']			= 57
globals()['KB_SCROLL_LOCK']		= 71
globals()['KB_NUM_LOCK']			= 83
globals()['KB_PRINT_SCREEN']		= 70
globals()['KB_PAUSE']				= 72
globals()['KB_F1']					= 58
globals()['KB_F2']					= 59
globals()['KB_F3']					= 60
globals()['KB_F4']					= 61
globals()['KB_F5']					= 62
globals()['KB_F6']					= 63
globals()['KB_F7']					= 64
globals()['KB_F8']					= 65
globals()['KB_F9']					= 66
globals()['KB_F10']					= 67
globals()['KB_F11']					= 68
globals()['KB_F12']					= 69
globals()['KB_F13']					= 104
globals()['KB_F14']					= 105
globals()['KB_F15']					= 106
globals()['KB_F16']					= 107
globals()['KB_F17']					= 108
globals()['KB_F18']					= 109
globals()['KB_F19']					= 110
globals()['KB_F20']					= 111
globals()['KB_F21']					= 112
globals()['KB_F22']					= 113
globals()['KB_F23']					= 114
globals()['KB_F24']					= 115
globals()['KB_KP_1']					= 89
globals()['KB_KP_2']					= 90
globals()['KB_KP_3']					= 91
globals()['KB_KP_4']					= 92
globals()['KB_KP_5']					= 93
globals()['KB_KP_6']					= 94
globals()['KB_KP_7']					= 95
globals()['KB_KP_8']					= 96
globals()['KB_KP_9']					= 97
globals()['KB_KP_0']					= 98
globals()['KB_KP_DECIMAL']			= 220
globals()['KB_KP_DIVIDE']			= 84
globals()['KB_KP_MULTIPLY']		= 85
globals()['KB_KP_SUBTRACT']		= 86
globals()['KB_KP_ADD']				= 87
globals()['KB_KP_ENTER']			= 88
globals()['KB_KP_EQUAL']			= 103
globals()['KB_LEFT_SHIFT']			= 225
globals()['KB_LEFT_CONTROL']		= 224
globals()['KB_LEFT_ALT']			= 226
globals()['KB_LEFT_SUPER']			= 227
globals()['KB_RIGHT_SHIFT']		= 229
globals()['KB_RIGHT_CONTROL']		= 228
globals()['KB_RIGHT_ALT']			= 230
globals()['KB_RIGHT_SUPER']		= 231
globals()['KB_MENU']					= 257

globals()['MOUSE_LEFT']				= 0
globals()['MOUSE_RIGHT']         = 1
globals()['MOUSE_MIDDLE']        = 2


globals()['ZERO']							= 0
globals()['ONE']							= 1
globals()['SRC_COLOR']					= 768
globals()['ONE_MINUS_SRC_COLOR']		= 769
globals()['DST_COLOR']					= 774
globals()['ONE_MINUS_DST_COLOR']		= 775
globals()['SRC_ALPHA']					= 770
globals()['ONE_MINUS_SRC_ALPHA']		= 771
globals()['DST_ALPHA']					= 772
globals()['ONE_MINUS_DST_ALPHA']		= 773
globals()['CONSTANT_COLOR']			= 32769
globals()['ONE_MINUS_CONSTANT_COLOR'] = 32770
globals()['SRC_ALPHA_SATURATE']		= 776
globals()['SRC1_COLOR']					= 35065
globals()['ONE_MINUS_SRC1_COLOR']	= 35066
globals()['SRC1_ALPHA']					= 34185
globals()['ONE_MINUS_SRC1_ALPHA']	= 35067


# blend presets (same as //developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation)

globals()['BLEND_SOURCE_OVER']		= 0
globals()['BLEND_SOURCE_IN']			= 1
globals()['BLEND_SOURCE_OUT']			= 2
globals()['BLEND_SOURCE_ATOP']		= 3
globals()['BLEND_DESTINATION_OVER']	= 4
globals()['BLEND_DESTINATION_IN']	= 5
globals()['BLEND_DESTINATION_OUT']	= 6
globals()['BLEND_DESTINATION_ATOP']	= 7
globals()['BLEND_LIGHTER']				= 8
globals()['BLEND_COPY']					= 9
globals()['BLEND_XOR']					= 10
globals()['BLEND_MULTIPLY']			= 11
globals()['BLEND_SCREEN']				= 12
globals()['BLEND_OVERLAY']				= 13
globals()['BLEND_DARKEN']				= 14
globals()['BLEND_LIGHTEN']				= 15
globals()['BLEND_COLOR_DODGE']		= 16
globals()['BLEND_COLOR_BURN']			= 17
globals()['BLEND_HARD_LIGHT']			= 18
globals()['BLEND_SOFT_LIGHT']			= 19
globals()['BLEND_DIFFERENCE']			= 20
globals()['BLEND_EXCLUSION']			= 21
globals()['BLEND_HUE']					= 22
globals()['BLEND_SATURATION']			= 23
globals()['BLEND_COLOR']				= 24
globals()['BLEND_LUMINOSITY']			= 25

globals()['TEX_LINEAR']					= 0
globals()['TEX_NEAREST']				= 1
globals()['TEX_CLAMP_TO_EDGE']      = 2
globals()['TEX_CLAMP_TO_BORDER']    = 3
globals()['TEX_MIRRORED_REPEAT']    = 4
globals()['TEX_REPEAT']      			= 5


globals()['game'] = sys.modules['game'] = game
globals()['assets'] = sys.modules['assets'] = assets
globals()['audio'] = sys.modules['audio'] = audio
globals()['renderer'] = sys.modules['renderer'] = renderer
globals()['config'] = sys.modules['config'] = config
globals()['textbox'] = sys.modules['textbox'] = textbox
globals()['keyboard'] = sys.modules['keyboard'] = keyboard
globals()['mouse'] = sys.modules['mouse'] = mouse
globals()['gamepad'] = sys.modules['gamepad'] = gamepad
globals()['shader'] = sys.modules['shader'] = shader
globals()['sprite_map'] = sys.modules['sprite_map'] = sprite_map

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


# stepper feature ------------------------------------------------------------

class Stepper:
	def __init__(self,delay, iv):
		self.delay = delay
		self.iter = 0
		self.value = iv
	def next(self, dt):
		self.iter += dt
		if self.iter >= self.delay:
			self.iter -= self.delay
			return True
		return False

def shim_create_stepper(delay, iv):
	return Stepper(delay, iv)
assets.create_stepper = shim_create_stepper

# init code ------------------------------------------------------------------

game.create_window()

# start: Scene Module --------------------------------------------------------

class ScenesManager:
    enabled=False
    current=None
    current_name=''
    entries={}

    def enable(self):
        self.enabled=True
    def disable(self):
        self.enabled=False
    
    def on(self, name, event, cb):
        if not name in self.entries:
            self.entries[name] = {}
        self.entries[name][event] = cb
    
    def trigger(self, event, arg=None):
        if self.enabled and self.current and event in self.current:
            if (arg):
                self.current[event](arg)
            else:
                self.current[event]()

    def select(self, name):
        if self.current:
            self.trigger('hidden')
        if self.entries[name]:
            self.current = self.entries[name]
            self.current_name = name
            self.trigger('shown')
    
    def remove(self, name):
        if self.entries[name]:
            self.entries[name] = None

globals()['scenes'] = ScenesManager()

def handle_game_update(dt):
    scenes.trigger('update', dt)

def handle_game_draw(dt):
    scenes.trigger('draw', dt)

globals()['handle_game_update'] = handle_game_update
globals()['handle_game_draw'] = handle_game_draw

game.on('update', 'handle_game_update')
game.on('draw', 'handle_game_draw')

# end: Scene Module ----------------------------------------------------------
)"; }
#endif
