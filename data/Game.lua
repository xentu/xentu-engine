--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]



-- called when the game first loads.
game.on("init", function()
	-- shader loading example
	-- shader1 = assets.load_shader("/custom-shader/vertex.shader", "/custom-shader/fragment.shader")
	-- game.use_shader(shader1)

	-- create a sprite.
	texture1 = assets.load_texture("images/texture1.png", TX_RGBA, TX_REPEAT)
	spritemap1 = assets.load_spritemap("images/texture1.json")
	sprite = Sprite(texture1, 100, 10, 150, 150)
	sprite.spritemap = spritemap1
	sprite.x_speed = 1

	-- create a font.
	texture2 = assets.load_texture("fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap2 = assets.load_spritemap("fonts/arial_20.json")
	font = Font(texture2, spritemap2)
	font.letter_spacing = 1
	font.line_height = 24

	-- load some audio.
    piano_c = assets.load_audio("audio/Piano_Hit2.mp3")
    piano_a = assets.load_audio("audio/Piano_Hit.mp3")
	stage5_loop = assets.load_audio("audio/8bit_Stage5_Loop.mp3");

	audio.play(stage5_loop)
	audio.set_volume(stage5_loop, 0)

	-- color class tests.
	local c = Color.fromHex('800080')
	print("Hex Form: #" .. c.toHex())
	renderer.set_clear_color(c)
	
    -- say hello.
    print("Hello from Lua world!")

	-- blend stuffs
	renderer.enbable_blend(true)
end)



-- the update event.
game.on("update", function()
	if sprite.x_speed > 0 and sprite.x + 1 > viewport.width - sprite.width then sprite.x_speed = -1 end
	if sprite.x_speed < 0 and sprite.x - 1 < 0 then sprite.x_speed = 1 end
	sprite.x = sprite.x + (sprite.x_speed * 2)
	
	-- test to show setting realtime volume
	if sprite.x % 50 == 0 then audio.set_volume(stage5_loop, sprite.x / viewport.width) end

	-- exit the game if the escape key is pressed
    if keyboard.key_down(KB_ESCAPE) or mouse.button_down(MB_LEFT) then game.exit() end
end)



-- the drawing event.
game.on("draw", function()
    renderer.begin()
    -- renderer.set_blend_preset(BLEND_SOURCE_IN) // use a preset for blending
    -- renderer.set_blend_func(SRC_ALPHA, ONE_MINUS_SRC_ALPHA) // alternatively set blending like you do in OpenGL
	-- renderer.set_origin(50, 50)
    -- renderer.set_rotation(sprite.rotation)
    -- renderer.set_scale(sprite.scale.x, sprite.scale.y)
    renderer.draw_sprite(sprite)

    renderer.draw_text(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed imperdiet tortor eget pulvinar eleifend. Maecenas dapibus mauris " ..
	                         "sed felis laoreet pellentesque. Nam eget diam a lacus semper placerat luctus mattis arcu. Cras ac iaculis quam. Maecenas arcu " ..
							 "ligula, congue eget ligula nec, pellentesque bibendum lacus. Cras iaculis lectus ipsum, euismod posuere sem rutrum sed.",
     						 100, 200, viewport.width - 200)
    renderer.present()
end)