## Function List

Here is a master list for the functions that should be available to each
of the implemented programming languages. 

### Game Functions

```
game_create_window()
game_path() -
game_fullscreen() -
game_set_fullscreen(bool) -
game_on(event, callback)
game_trigger(event)
game_run()
game_exit() -
```


### Geometry

```
geometry_create_rect(x, y, width, height)
geometry_create_polygon(coords) -
```


### Asset Functions

```
assets_mount(virtual_path, real_path)
assets_read_text_file(path)
assets_load_texture(path)
assets_load_audio(path) -
assets_load_font(path) -
```

### Renderer Functions

```
renderer_clear()
renderer_clear(color) - 
renderer_draw_sprite(rect, texture)
renderer_draw_rect(color, x, y, width, height)
renderer_draw_text(font, text, x, y, max_width) -
renderer_set_clear_color(color)
```

### Input Functions

```
keyboard_key_clicked(key) -
keyboard_key_down(key) -
```