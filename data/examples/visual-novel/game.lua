--[[
	Visual Novel Example
]]
scenes = {}
fonts = {}
colors = {
    menuitem = Color.from_hex('#AAAAAA'),
    bg = Color.from_hex('#333333'),
    white = Color.from_hex("#ffffff")
}


game.on('init', function()
    -- create a font.
	font_tex1 = assets.load_texture("fonts/arial_20.png")
	font_sm1 = assets.load_spritemap("fonts/arial_20.json")
	fonts.arial_20 = Font(font_tex1, font_sm1)

    -- load the scenes
    scenes.menu = include('scenes.menu')
    scenes.chapter1 = include('scenes.chapter1')

    -- set some rendering variables.
	renderer.set_clear_color(colors.bg)
	renderer.set_blend(false)

    -- set the initial scene to use
    game.set_scene(scenes.menu.handle)
end)