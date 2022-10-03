## Function List

Here is a master list for the functions that should be available to each
of the implemented programming languages. 

### Global Functions
print(text)

### Game Functions
```
game_create_window()
game_on(event, callback)
game_trigger(event)
game_run()
game_exit()
```

### Geometry
```
geometry_create_rect(x, y, width, height)
```

### Asset Functions
```
assets_mount(virtual_path, real_path)
assets_read_text_file(path)
assets_load_texture(path)
assets_load_font(path, font_size)
assets_load_sound(path)
assets_load_music(path)
assets_load_shader(vertex_text, fragment_text)
assets_create_textbox(x,y,w,h)
assets_include(path)
```

### Audio Functions
```
audio_play_sound(sound_id, channel, loops)
audio_play_music(music_id, loops)
audio_stop_music()
audio_stop_sounds(channel) 
audio_set_sound_volume(id, volume)
audio_set_channel_volume(channel, volume)
audio_set_music_volume(volume)
audio_set_channel_panning(channel, left, right)
```

### Renderer Functions
```
renderer_begin()
renderer_clear()
renderer_present()
renderer_draw_texture(color, x, y, width, height)
renderer_draw_sub_texture(color, x, y, w, h, sx, sy, sw, sh)
renderer_draw_rectangle(x, y, w, h)
renderer_draw_textbox(textbox_id)
renderer_set_background(color)
renderer_set_foreground(color)
renderer_set_window_mode(mode)
renderer_set_position(x,y)
renderer_set_origin(x,y)
renderer_set_rotation(angle)
renderer_set_scale(x,y)
renderer_set_shader(shader_id) ~
```

### Shader Functions
```
shader_get_uniform(shader_id, name) ~
shader_set_uniform_1b(shader_id, uniform_id, b0) ~
shader_set_uniform_1i(shader_id, uniform_id, i0) ~
shader_set_uniform_2i(shader_id, uniform_id, i0, i1) ~
shader_set_uniform_3i(shader_id, uniform_id, i0, i1, i2) ~
shader_set_uniform_4i(shader_id, uniform_id, i0, i1, i2, i3) ~
shader_set_uniform_1f(shader_id, uniform_id, f0) ~
shader_set_uniform_2f(shader_id, uniform_id, f0, f1) ~
shader_set_uniform_3f(shader_id, uniform_id, f0, f1, f2) ~
shader_set_uniform_4f(shader_id, uniform_id, f0, f1, f2, f3) ~
shader_set_uniform_mat4f(shader_id, uniform_id, matrix) ~
```

### Config Functions
```
config_get_str
config_get_str2
config_get_bool
config_get_bool2
config_get_int
config_get_int2
```

### Object Functions
```
textbox_set_text(textbox_id, text)
textbox_set_color(color)
textbox_measure_text(textbox_id, text) ~
```

### Input Functions
```
keyboard_key_down(key)
keyboard_key_clicked(key)
```

### Data Functions (Encoding/Compression)
```
data_to_base16(bytes)
data_to_base32(bytes)
data_to_base64(bytes)
data_to_xml(any)
data_to_json(any)
data_to_gzip(bytes)
data_to_zlib(bytes)

data_from_base16(string)
data_from_base32(string)
data_from_base64(string)
data_from_xml(any)
data_from_json(any)
data_from_gzip(bytes)
data_from_zlib(bytes)

data_create_guid(seed)
data_create_hash(algorithm, data)
```