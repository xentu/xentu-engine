--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]


-- called when the game first loads.
game.on("init", function()
	-- create a spritemap.
	texture1 = assets.load_texture("../data/images/texture1.png", TX_RGBA, TX_REPEAT)
	spritemap1 = assets.load_spritemap("../data/images/texture1.json")
	-- spritemap1 = assets.load_spritemap("images/texture1.json", texture1)


    -- create a sprite (x,y,z,key).
	--sprite = Sprite(0, 0, 50, 50, "full")
	--sprite.x_speed = 1
		
	-- create a font.
	--texture2 = assets.load_texture("fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	--font = assets.load_spritemap("fonts/arial_20.json", texture2)
	
    -- say hello.
    game.log("Hello from Lua world!")
	game.log("texture1 = " .. texture1)
	game.log("spritemap1 = " .. spritemap1)
end)


-- the update event.
game.on("update", function()
	-- if s1.speedx > 0 and s1.x + 1 > viewport.width then s1.speedx = -1 end
	-- if s1.speedx < 0 and s1.x - 1 < 0 then s1.speedx = 1 end
	-- if s1.x = 0 then audio.play(piano_c) end
	-- if s1.x = viewport.width then audio.play(piano_a) end
    -- if keyboard.clicked(KB_ESCAPE) or mouse.button_down(MB_LEFT) then game.exit() end
end)


-- the drawing event.
game.on("draw", function()
    renderer.begin()
	renderer.draw_sprite(texture1, spritemap1, "full", 25, 25, 50, 50)
    --renderer.draw_sprite(sprite, spritemap, 0, 0, 50, 50)
    --renderer.draw_text(font, "this is an awesome test.", 100, 200, viewport.width - 100)
    renderer.present()
end)