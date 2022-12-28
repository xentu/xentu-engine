## Function List

Here is a master list for the functions that should be available to each
of the implemented programming languages. 
Any function with a ~ at the end is being worked on and not yet available.


### Global Functions
```
include(path)
print(text)
```

### Game Functions
```
game_on(event, callback)
game_trigger(event, [arg])
game_exit()
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
assets_load_sprite_map(path)
assets_create_textbox(x,y,w,h)
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
renderer_clear()
renderer_begin(reset)
renderer_present()
renderer_draw_texture(texture_id, x, y, width, height)
renderer_draw_sub_texture(texture_id, x, y, w, h, sx, sy, sw, sh)
renderer_draw_rectangle(x, y, w, h)
renderer_draw_textbox(textbox_id)
renderer_draw_sprite(sprite_map_id, region_name, frame, x, y, w, h)
renderer_set_background(color)
renderer_set_foreground(color)
renderer_set_window_mode(mode)
renderer_set_position(x,y)
renderer_set_origin(x,y)
renderer_set_rotation(angle)
renderer_set_scale(x,y)
renderer_set_shader(shader_id)
renderer_set_alpha(alpha)
renderer_set_blend(bool)
renderer_set_blend_func(src_mode, dest_mode)
renderer_set_blend_preset(preset)
```

### Sprite Maps
```
sprite_map_set_region(sprite_map_id, region_name, x, y, w, h)
sprite_map_set_texture(texture_id)
sprite_map_reset(sprite_map_id)
```

### Shader Functions
```
shader_get_uniform(name)
shader_set_uniforms_bool(uniform_id, b0...)
shader_set_uniforms_int(uniform_id, i0...)
shader_set_uniforms_float(uniform_id, f0...)
shader_set_uniform_matrix(uniform_id, matrix) ~
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

### TextBox Functions
```
textbox_set_text(textbox_id, text)
textbox_set_color(color)
textbox_measure_text(textbox_id, text)
```

### Input Functions
```
keyboard_key_down(key)
keyboard_key_clicked(key)
mouse_get_position() ~
mouse_button_down(button) ~
mouse_button_click(button) ~
```

### TileMap Functions

Aiming to add these in the next few weeks.

Signature | Description
--------- | -----------
assets_load_tilemap(path)                        | loads a tilemap into memory.
assets_unload_tilemap(tm)                        | unload a tilemap from memory.
tilemap_get_layer(tm, layer)                     | gets layer properties.
tilemap_get_layer_tiles(tm, layer)               | gets layer tile data.
tilemap_get_layer_objects(tm, layer)             | gets layer objects.
tilemap_get_layer_polygons(tm, layer)            | gets layer polygons.
tilemap_get_tile(tm, layer, tile)                | gets tile properties.
tilemap_get_object(tm, layer, object)            | gets object properties.
tilemap_change_layer(tm, layer, prop, val)       | change the prop for a layer.
tilemap_change_tile(tm, layer, tile, prop, val)  | change the prop for a tile on a layer.
tilemap_change_object(tm, layer, obj, prop, val) | change the prop for an object on a layer.
renderer_draw_tile_layer(tm, index)              | draws a tilemap layer (tiles & polygons).

### Data Functions (Encoding/Compression)
```
data_to_base16(bytes) ~
data_to_base32(bytes) ~
data_to_base64(bytes) ~
data_to_xml(any) ~
data_to_json(any) ~
data_to_gzip(bytes) ~
data_to_zlib(bytes) ~

data_from_base16(string) ~
data_from_base32(string) ~
data_from_base64(string) ~
data_from_gzip(bytes) ~
data_from_zlib(bytes) ~

data_create_guid(seed) ~
data_create_hash(algorithm, data) ~
```