--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]


-- called when the game first loads.
game.on("init", function()
	-- create a sprite.
	texture1 = assets.load_texture("/images/texture1.png", TX_RGBA, TX_REPEAT)
	spritemap1 = assets.load_spritemap("/images/texture1.json")
	sprite = Sprite(texture1, 100, 10, 150, 150)
	sprite.spritemap = spritemap1
	sprite.x_speed = 1
		
	-- create a font.
	texture2 = assets.load_texture("/fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap2 = assets.load_spritemap("/fonts/arial_20.json")
	font = Font(texture2, spritemap2)
	font.letter_spacing = 1
	font.line_height = 24

	-- load some audio.
    piano_c = assets.load_audio("/audio/Piano_Hit2.mp3")
    piano_a = assets.load_audio("/audio/Piano_Hit.mp3")
	
    -- say hello.
    game.log("Hello from Lua world!")
end)


-- the update event.
game.on("update", function()
	if sprite.x == 0 then game.audio.play(piano_c) end
	if sprite.x == viewport.width - sprite.width then game.audio.play(piano_a) end

	if sprite.x_speed > 0 and sprite.x + 1 > viewport.width - sprite.width then sprite.x_speed = -1 end
	if sprite.x_speed < 0 and sprite.x - 1 < 0 then sprite.x_speed = 1 end
	sprite.x = sprite.x + (sprite.x_speed * 2)
	
    -- if keyboard.clicked(KB_ESCAPE) or mouse.button_down(MB_LEFT) then game.exit() end
end)


-- the drawing event.
game.on("draw", function()
    renderer.begin()
    renderer.draw_sprite(sprite)

    renderer.draw_text(font, "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed imperdiet tortor eget pulvinar eleifend. Maecenas dapibus mauris " ..
	                         "sed felis laoreet pellentesque. Nam eget diam a lacus semper placerat luctus mattis arcu. Cras ac iaculis quam. Maecenas arcu " ..
							 "ligula, congue eget ligula nec, pellentesque bibendum lacus. Cras iaculis lectus ipsum, euismod posuere sem rutrum sed.",
    						 100, 200, viewport.width - 200)
    renderer.present()
end)