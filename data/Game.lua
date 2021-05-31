--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]


-- called when the game first loads.
game.on("init", function()
	-- create a sprite.
	texture1 = assets.load_texture("../data/images/texture1.png", TX_RGBA, TX_REPEAT)
	spritemap1 = assets.load_spritemap("../data/images/texture1.json")
	sprite = Sprite(texture1, 100, 10, 150, 150)
	sprite.spritemap = spritemap1
	sprite.x_speed = 1
		
	-- create a font.
	texture2 = assets.load_texture("../data/fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	font = assets.load_spritemap("../data/fonts/arial_20.json")
	
    -- say hello.
    game.log("Hello from Lua world!")
end)


-- the update event.
game.on("update", function()
	if sprite.x_speed > 0 and sprite.x + 1 > viewport.width - sprite.width then sprite.x_speed = -1 end
	if sprite.x_speed < 0 and sprite.x - 1 < 0 then sprite.x_speed = 1 end
	sprite.x = sprite.x + (sprite.x_speed * 2)
	-- if s1.x = 0 then audio.play(piano_c) end
	-- if s1.x = viewport.width then audio.play(piano_a) end
    -- if keyboard.clicked(KB_ESCAPE) or mouse.button_down(MB_LEFT) then game.exit() end
end)


-- the drawing event.
game.on("draw", function()
    renderer.begin()
    renderer.draw_sprite(sprite)
    renderer.draw_text(texture2, font, "this is an awesome test.", 100, 200, viewport.width - 100)
    renderer.present()
end)