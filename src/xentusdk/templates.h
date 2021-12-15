#include <string>

const char* lua_template = "\
--[[ \n\
   {title} \n\
   -------------------------- \n\
   Put a nice description of your game here. For info visit xentu.net/docs \n\
]] \n\
\n\
\n\
-- called when the game first loads. \n\
game.on('init', function() \n\
    -- set a background color \n\
    local c = Color.from_hex('#000000') \n\
    renderer.set_clear_color(c) \n\
    renderer.set_blend(true) \n\
    \n\
    -- load a texture \n\
    texture1 = assets.load_texture('logo.png', TX_RGBA, TX_CLAMP_TO_BORDER) \n\
    -- make a sprite using the texture (x,y,w,h) \n\
    sprite = Sprite(texture1, 336, 100, 128, 128) \n\
    \n\
    -- say hello! \n\
    print('Hello from Lua world!') \n\
end) \n\
\n\
game.on('draw', function() \n\
    -- render the texture \n\
    renderer.begin() \n\
    renderer.draw_sprite(sprite) \n\
    renderer.present() \n\
end) \n\
";

const char* toml_template = "\
[general]  \n\
game_title = '{title}'\n\
game_author = '{author}' \n\
screen_width = 800  \n\
screen_height = 600  \n\
update_frequency = 60 \n\
vsync = true \n\
fullscreen = false \n\
 \n\
[viewport] \n\
width = 800 \n\
height = 600 \n\
";


