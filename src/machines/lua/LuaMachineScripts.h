#pragma once
namespace xen { 
const int xen_lua_script_init_c = 4;
const char* xen_lua_script_init[xen_lua_script_init_c] = {
R"(interop = XentuLuaMachineInterop()

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
assets.load_sound = function(path) return interop.assets_load_sound(path) end
assets.load_music = function(path) return interop.assets_load_music(path) end
assets.load_shader = function(a,b) return interop.assets_load_shader(a,b) end
assets.load_sprite_map = function(path) return interop.assets_load_sprite_map(path) end
assets.load_tile_map_tmx = function(path, working_dir) return interop.assets_load_tile_map_tmx(path, working_dir) end
assets.create_textbox = function(a, b) return interop.assets_create_textbox(a, b) end
include = function(path)
	code = assets.read_text_file(path)
	delegate = load(code)
	delegate()
end
assets.unload_texture = function(a) return interop.assets_unload_texture(a) end
assets.unload_font = function(a) return interop.assets_unload_font(a) end
assets.unload_sound = function(a) return interop.assets_unload_sound(a) end
assets.unload_music = function(a) return interop.assets_unload_music(a) end
assets.unload_shader = function(a) return interop.assets_unload_shader(a) end
assets.unload_sprite_map = function(a) return interop.assets_unload_sprite_map(a) end
assets.unload_tile_map = function(a) return interop.assets_unload_tile_map(a) end
assets.set_wrap = function(...) return interop.assets_set_wrap(...) end
assets.set_interpolation = function(...) return interop.assets_set_interpolation(...) end

audio = {}
audio.play_sound = function(sound_id, channel, loops) interop.audio_play_sound(sound_id, channel, loops) end
audio.play_music = function(music_id, loops) interop.audio_play_music(music_id, loops) end
audio.stop_sound = function(channel_id) interop.audio_stop_sound(channel_id) end
audio.stop_music = function() interop.audio_stop_music() end
audio.set_sound_volume = function(sound_id,vol) interop.audio_set_sound_volume(sound_id,vol) end
audio.set_channel_volume = function(channel_id,vol) interop.audio_set_channel_volume(channel_id,vol) end
audio.set_music_volume = function(music_id,vol) interop.audio_set_music_volume(music_id,vol) end
audio.set_channel_panning = function(id,left,right) interop.audio_set_channel_panning(id,left,right) end

renderer = {}
renderer.begin = function(reset) interop.renderer_begin(reset) end
renderer.clear = function() interop.renderer_clear() end
renderer.present = function() interop.renderer_present() end
renderer.draw_texture = function(a, b, c, d, e) interop.renderer_draw_texture(a, b, c, d, e) end
renderer.draw_sub_texture = function(a, b,c,d,e, f,g,h,i) interop.renderer_draw_sub_texture(a, b,c,d,e, f,g,h,i) end
renderer.draw_rectangle = function(a,b,c,d) interop.renderer_draw_rectangle(a,b,c,d) end
renderer.draw_textbox = function(a, x, y) interop.renderer_draw_textbox(a, x, y) end
renderer.draw_sprite = function(id,region,frame,x,y,w,h) interop.renderer_draw_sprite(id,region,frame,x,y,w,h) end
renderer.draw_tile_layer = function(id,layer) interop.renderer_draw_tile_layer(id,layer) end
renderer.set_background = function(hex)
	hex2 = hex:gsub("#", "")
	interop.renderer_set_background(hex2)
end
renderer.set_foreground = function(hex)
	hex2 = hex:gsub("#", "")
	interop.renderer_set_foreground(hex2)
end
renderer.set_window_mode = function(mode) interop.renderer_set_window_mode(mode) end
renderer.set_position = function(x,y) interop.renderer_set_position(x,y) end
renderer.set_origin = function(x,y) interop.renderer_set_origin(x,y) end
renderer.set_rotation = function(angle) interop.renderer_set_rotation(angle) end
renderer.set_scale = function(x,y) interop.renderer_set_scale(x,y) end
renderer.set_shader = function(x) interop.renderer_set_shader(x) end
renderer.set_alpha = function(alpha) interop.renderer_set_alpha(alpha) end
renderer.set_blend = function(enable) interop.renderer_set_blend(enable) end
renderer.set_blend_func = function(src, dst) interop.renderer_set_blend_func(src, dst) end
renderer.set_blend_preset = function(src, dst) interop.renderer_set_blend_preset(mode, p_alpha) end

sprite_map = {}
sprite_map.get_frame_info = function(id,region,frame) return interop.sprite_map_get_frame_info(id,region,frame) end
sprite_map.get_frame_count = function(id,region) return interop.sprite_map_get_frame_count(id,region) end

config = {}
config.get_str = function(a,b,c) return interop.config_get_str(a,b,c) end
config.get_str2 = function(a,b,c,d) return interop.config_get_str2(a,b,c,d) end
config.get_bool = function(a,b,c) return interop.config_get_bool(a,b,c) end
config.get_bool2 = function(a,b,c,d) return interop.config_get_bool2(a,b,c,d) end
config.get_int = function(a,b,c) return interop.config_get_int(a,b,c) end
config.get_int2 = function(a,b,c,d) return interop.config_get_int2(a,b,c,d) end

textbox = {}
textbox.set_text = function(a,b,c) interop.textbox_set_text(a,b,c) end
textbox.set_color = function(a,b,hex)
	hex2 = hex:gsub("#", "")
	interop.textbox_set_color(a,b,hex2)
end
textbox.measure_text = function(a,b,c) return interop.textbox_measure_text(a,b,c) end

keyboard = {}
keyboard.key_down = function(a) return interop.keyboard_key_down(a) end
keyboard.key_clicked = function(a) return interop.keyboard_key_clicked(a) end

mouse = {}
mouse.get_position = function() return interop.mouse_get_position() end
mouse.button_down = function(a) return interop.mouse_button_down(a) end
mouse.button_clicked = function(a) return interop.mouse_button_clicked(a) end

gamepad = {}
gamepad.get_axis = function(id) return interop.gamepad_get_axis(id) end
gamepad.button_down = function(id,bt) return interop.gamepad_button_down(id,bt) end
gamepad.button_clicked = function(id,bt) return interop.gamepad_button_clicked(id,bt) end

shader = {}
shader.get_location = function(var_name) return interop.shader_get_location(var_name) end
shader.set_bool = function(...) return interop.shader_set_bool(arg) end
shader.set_int = function(...) return interop.shader_set_int(arg) end
shader.set_float = function(...) return interop.shader_set_float(arg) end


-- keyboard codes ---------------------------------------------------------

KB_SPACE					= 44
KB_APOSTROPHE			= 52
KB_COMMA					= 54
KB_MINUS					= 45
KB_PERIOD				= 55
KB_SLASH					= 56
KB_1						= 30
KB_2						= 31
KB_3						= 32
KB_4						= 33
KB_5						= 34
KB_6						= 35
KB_7						= 36
KB_8						= 37
KB_9						= 38
KB_0						= 39
KB_SEMICOLON			= 51
KB_EQUAL					= 103
KB_A						= 4
KB_B						= 5
KB_C						= 6
KB_D						= 7
KB_E						= 8
KB_F						= 9
KB_G						= 10
KB_H						= 11
KB_I						= 12
KB_J						= 13
KB_K						= 14
KB_L						= 15
KB_M						= 16
KB_N						= 17
KB_O						= 18
KB_P						= 19
KB_Q						= 20
KB_R						= 21
KB_S						= 22
KB_T						= 23
KB_U						= 24
KB_V						= 25
KB_W						= 26
KB_X						= 27
KB_Y						= 28
KB_Z						= 29
KB_LEFT_BRACKET		= 184
KB_BACKSLASH			= 100
KB_RIGHT_BRACKET		= 185
KB_GRAVE_ACCENT		= 53
KB_ESCAPE				= 41
KB_ENTER					= 88
KB_RETURN            = 40
KB_TAB					= 186
KB_BACKSPACE			= 187
KB_INSERT				= 73
KB_DELETE				= 76
KB_RIGHT					= 79
KB_LEFT					= 80
KB_DOWN					= 81
KB_UP						= 82
KB_PAGE_UP				= 75
KB_PAGE_DOWN			= 78
KB_HOME					= 74
KB_END					= 77
KB_CAPS_LOCK			= 57
KB_SCROLL_LOCK			= 71
KB_NUM_LOCK				= 83
KB_PRINT_SCREEN		= 70
KB_PAUSE					= 72
KB_F1						= 58
KB_F2						= 59
KB_F3						= 60
KB_F4						= 61
KB_F5						= 62
KB_F6						= 63
KB_F7						= 64
KB_F8						= 65
KB_F9						= 66
KB_F10					= 67
KB_F11					= 68
KB_F12					= 69
KB_F13					= 104
KB_F14					= 105
KB_F15					= 106
KB_F16					= 107
KB_F17					= 108
KB_F18					= 109
KB_F19					= 110
KB_F20					= 111
KB_F21					= 112
KB_F22					= 113
KB_F23					= 114
KB_F24					= 115
KB_KP_1					= 89
KB_KP_2					= 90
KB_KP_3					= 91
KB_KP_4					= 92
KB_KP_5					= 93
KB_KP_6					= 94
KB_KP_7					= 95
KB_KP_8					= 96
KB_KP_9					= 97
KB_KP_0					= 98
KB_KP_DECIMAL			= 220
KB_KP_DIVIDE			= 84
KB_KP_MULTIPLY			= 85
KB_KP_SUBTRACT			= 86
KB_KP_ADD				= 87
KB_KP_ENTER				= 88
KB_KP_EQUAL				= 103
KB_LEFT_SHIFT			= 225
KB_LEFT_CONTROL		= 224
KB_LEFT_ALT				= 226
KB_LEFT_SUPER			= 227
KB_RIGHT_SHIFT			= 229
KB_RIGHT_CONTROL		= 228
KB_RIGHT_ALT			= 230
KB_RIGHT_SUPER			= 231
KB_MENU					= 257


-- mouse codes ------------------------------------------------------------

MOUSE_LEFT				= 0
MOUSE_RIGHT				= 1
MOUSE_MIDDLE         = 2


-- blend factor constants (from opengl) -----------------------------------

ZERO 							 = 0
ONE							 = 1
SRC_COLOR					 = 768 --0x0300
ONE_MINUS_SRC_COLOR		 = 769 --0x0301
DST_COLOR					 = 774 --0x0306
ONE_MINUS_DST_COLOR		 = 775 --0x0307
SRC_ALPHA					 = 770 --0x0302
ONE_MINUS_SRC_ALPHA		 = 771 --0x0303
DST_ALPHA					 = 772 --0x0304
ONE_MINUS_DST_ALPHA		 = 773 --0x0305
CONSTANT_COLOR				 = 32769 --0x8001
ONE_MINUS_CONSTANT_COLOR = 32770 --0x8002
SRC_ALPHA_SATURATE		 = 776 --0x0308
SRC1_COLOR					 = 35065 --0x88F9
ONE_MINUS_SRC1_COLOR		 = 35066 --0x88FA
SRC1_ALPHA					 = 34185 --0x8589
ONE_MINUS_SRC1_ALPHA		 = 35067 --0x88FB


-- blend presets (same as //developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/globalCompositeOperation)

BLEND_SOURCE_OVER			 = 0
BLEND_SOURCE_IN			 = 1
BLEND_SOURCE_OUT			 = 2
BLEND_SOURCE_ATOP			 = 3
BLEND_DESTINATION_OVER	 = 4
BLEND_DESTINATION_IN		 = 5
BLEND_DESTINATION_OUT	 = 6
BLEND_DESTINATION_ATOP	 = 7
BLEND_LIGHTER				 = 8
BLEND_COPY					 = 9
BLEND_XOR					 = 10
BLEND_MULTIPLY				 = 11
BLEND_SCREEN				 = 12
BLEND_OVERLAY				 = 13
BLEND_DARKEN				 = 14
BLEND_LIGHTEN				 = 15
BLEND_COLOR_DODGE			 = 16
BLEND_COLOR_BURN			 = 17
BLEND_HARD_LIGHT			 = 18
BLEND_SOFT_LIGHT			 = 19
BLEND_DIFFERENCE			 = 20
BLEND_EXCLUSION			 = 21
BLEND_HUE					 = 22
BLEND_SATURATION			 = 23
BLEND_COLOR					 = 24
BLEND_LUMINOSITY			 = 25


TEX_LINEAR					 = 0
TEX_NEAREST					 = 1
TEX_CLAMP_TO_EDGE			 = 2
TEX_CLAMP_TO_BORDER		 = 3
TEX_MIRRORED_REPEAT		 = 4
TEX_REPEAT					 = 5


-- stepper feature -----------------------------------------------------------


assets.create_stepper = function(delay, iv)
	local s = {}
	s.delay = delay; s.iter = 0; s.value = iv;
	s.next = function(dt)
		s.iter = s.iter + dt
		if s.iter >= s.delay then
			s.iter = s.iter - s.delay
			return true
		end
		return false
	end
	return s
end


-- init code -----------------------------------------------------------------


game.create_window())",
R"(-- start: Scene Module -------------------------------------------------------

scenes = { enabled=false, current=nil, current_name='', entries={} }
scenes.init = function()
	game.on('update', function(dt) scenes.trigger('update', dt) end)
	game.on('draw', function(dt) scenes.trigger('draw', dt) end)
end
scenes.enable = function()
	scenes.enabled = true
end
scenes.disable = function()
	scenes.disable = true
end
scenes.on = function(name, event, cb)
    if scenes.entries[name] == nil then
        scenes.entries[name] = {}
    end
	scenes.entries[name][event] = cb
end
scenes.trigger = function(event, arg)
	if scenes.enabled and scenes.current and scenes.current[event] then
    	scenes.current[event](arg)
    end
end
scenes.select = function(name)
	if scenes.current  ~= nil then
      scenes.trigger('hidden')
	end
	if scenes.entries[name] ~= nil then
		scenes.current = scenes.entries[name]
		scenes.current_name = name
		scenes.trigger('shown')
	end
end
scenes.remove = function(name)
	if scenes.entries[name] ~= nil then
		if scenes.current_name == name then
			scenes.current = nil
			scenes.current_name = ''
		end
		scenes.entries[name] = nil
	end
end

scenes.init()

-- end: Scene Module ---------------------------------------------------------)",
R"(-- start: JSON Module --------------------------------------------------------
-- Stripped down version of gist.github.com/tylerneylon/59f4bcf316be525b30ab

json = {}

local function kind_of(obj)
  if type(obj) ~= 'table' then return type(obj) end
  local i = 1
  for _ in pairs(obj) do
    if obj[i] ~= nil then i = i + 1 else return 'table' end
  end
  if i == 1 then return 'table' else return 'array' end
end

local function escape_str(s)
  local in_char  = {'\\', '"', '/', '\b', '\f', '\n', '\r', '\t'}
  local out_char = {'\\', '"', '/',  'b',  'f',  'n',  'r',  't'}
  for i, c in ipairs(in_char) do
    s = s:gsub(c, '\\' .. out_char[i])
  end
  return s
end

local function skip_delim(str, pos, delim, err_if_missing)
  pos = pos + #str:match('^%s*', pos)
  if str:sub(pos, pos) ~= delim then
    if err_if_missing then
      error('Expected ' .. delim .. ' near position ' .. pos)
    end
    return pos, false
  end
  return pos + 1, true
end

local function parse_str_val(str, pos, val)
  val = val or ''
  local early_end_error = 'End of input found while parsing string.'
  if pos > #str then error(early_end_error) end
  local c = str:sub(pos, pos)
  if c == '"'  then return val, pos + 1 end
  if c ~= '\\' then return parse_str_val(str, pos + 1, val .. c) end
  local esc_map = {b = '\b', f = '\f', n = '\n', r = '\r', t = '\t'}
  local nextc = str:sub(pos + 1, pos + 1)
  if not nextc then error(early_end_error) end
  return parse_str_val(str, pos + 2, val .. (esc_map[nextc] or nextc))
end

local function parse_num_val(str, pos)
  local num_str = str:match('^-?%d+%.?%d*[eE]?[+-]?%d*', pos)
  local val = tonumber(num_str)
  if not val then error('Error parsing number at position ' .. pos .. '.') end
  return val, pos + #num_str
end

function json.stringify(obj, as_key)
  local s = {}
  local kind = kind_of(obj)
  if kind == 'array' then
    if as_key then error('Can\'t encode array as key.') end
    s[#s + 1] = '['
    for i, val in ipairs(obj) do
      if i > 1 then s[#s + 1] = ', ' end
      s[#s + 1] = json.stringify(val)
    end
    s[#s + 1] = ']'
  elseif kind == 'table' then
    if as_key then error('Can\'t encode table as key.') end
    s[#s + 1] = '{'
    for k, v in pairs(obj) do
      if #s > 1 then s[#s + 1] = ', ' end
      s[#s + 1] = json.stringify(k, true)
      s[#s + 1] = ':'
      s[#s + 1] = json.stringify(v)
    end
    s[#s + 1] = '}'
  elseif kind == 'string' then
    return '"' .. escape_str(obj) .. '"'
  elseif kind == 'number' then
    if as_key then return '"' .. tostring(obj) .. '"' end
    return tostring(obj)
  elseif kind == 'boolean' then
    return tostring(obj)
  elseif kind == 'nil' then
    return 'null'
  else
    error('Unjsonifiable type: ' .. kind .. '.')
  end
  return table.concat(s)
end

json.null = {}  -- This is a one-off table to represent the null value.

function json.parse(str, pos, end_delim)
  pos = pos or 1
  if pos > #str then error('Reached unexpected end of input.') end
  local pos = pos + #str:match('^%s*', pos)
  local first = str:sub(pos, pos)
  if first == '{' then
    local obj, key, delim_found = {}, true, true
    pos = pos + 1
    while true do
      key, pos = json.parse(str, pos, '}')
      if key == nil then return obj, pos end
      if not delim_found then error('Comma missing between object items.') end
      pos = skip_delim(str, pos, ':', true)
      obj[key], pos = json.parse(str, pos)
      pos, delim_found = skip_delim(str, pos, ',')
    end
  elseif first == '[' then
    local arr, val, delim_found = {}, true, true
    pos = pos + 1
    while true do
      val, pos = json.parse(str, pos, ']')
      if val == nil then return arr, pos end
      if not delim_found then error('Comma missing between array items.') end
      arr[#arr + 1] = val
      pos, delim_found = skip_delim(str, pos, ',')
    end
  elseif first == '"' then
    return parse_str_val(str, pos + 1)
  elseif first == '-' or first:match('%d') then
    return parse_num_val(str, pos)
  elseif first == end_delim then
    return nil, pos + 1
  else
    local literals = {['true'] = true, ['false'] = false, ['null'] = json.null}
    for lit_str, lit_val in pairs(literals) do
      local lit_end = pos + #lit_str - 1
      if str:sub(pos, lit_end) == lit_str then return lit_val, lit_end + 1 end
    end
    local pos_info_str = 'position ' .. pos .. ': ' .. str:sub(pos, pos + 10)
    error('Invalid json syntax starting at ' .. pos_info_str)
  end
end

-- end: JSON Module ----------------------------------------------------------)",
R"(-- start: Story Module -------------------------------------------------------

assets.load_story = function(path)
	text = assets.read_text_file(path)
	story = {
		 _text = text,
		 _callbacks = {},
		 path = path,
		 moments = json.parse(text),
		 cursor = 0,
		 current_moment = nil
	}

	-- find the index of a moment by its id, returns 0 on fail.
	story.find_index = function(moment_id)
		 if (moment_id == null) then return 0 end
		 for i=1,story.moments.length do
			  if story.moments[i].id and story.moments[i].id == moment_id then
					return i
			  end
		 end
		 return 0
	end

	-- move the cursor to a specific moment (by id).
	story.goto_moment = function(moment_id)
		 index = moment_id == nil and 1 or story.find_index(moment_id)
		 if index >= 0 then
			  story.goto_index(index)
		 end
	end

	-- goto a specific moment index.
	story.goto_index = function(index)
		 story.cursor = index
		 story.current_moment = story.moments[index]
		 -- trigger prompt event.
		 story.trigger('moment', story.current_moment)
	end

	-- move the cursor to the next moment.
	story.next = function()
		 next = story.cursor + 1
		 if story.moments[next] ~= nil then
			  story.goto_index(next)
		 end
	end

	-- completely reset the story.
	story.reset = function()
		 story.moments = json.parse(text)
		 story.trigger('reset', null)
		 story.goto_index(1)
	end

	-- trigger a story event.
	story.trigger = function(event, data)
		 if story._callbacks[event] ~= nil then
			  cb = story._callbacks[event]
			  for i,v in ipairs(cb) do
					cb[i](data)
			  end
		 end
	end

	-- handle various story events (moment, reset)
	story.on = function(event, cb)
		 if not story._callbacks[event] ~= nil then
			  story._callbacks[event] = {}
		 end
		 table.insert(story._callbacks[event], cb)
	end

	story.start = function()
		 story.goto_moment(null)
	end
	
	return story
end

-- end: Scene Module ---------------------------------------------------------)"
}; }