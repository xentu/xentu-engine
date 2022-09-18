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
assets_create_textbox(x,y,w,h)
```

### Renderer Functions
```
renderer_begin()
renderer_present()
renderer_draw_texture(color, x, y, width, height)
renderer_draw_sub_texture(color, x, y, w, h, sx, sy, sw, sh)
renderer_draw_textbox(textbox_id)
renderer_set_background(color)
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
```

### Input Functions
```
keyboard_key_down(key)
```