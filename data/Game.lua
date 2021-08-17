--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]



-- called when the game first loads.
game.on("init", function()
	-- load a tilemap
	map = assets.load_tilemap("tiled-desert/desert.tmx")
	layer0 = map.get_layer(0)

	-- set the background.
	renderer.set_clear_color( Color.fromHex('#5f7988') )
	
    print("Hello from Lua world!")
	renderer.set_blend(true)
end)



-- the update event.
game.on("update", function()
	-- do nothing for now
end)



-- the drawing event.
game.on("draw", function()
    renderer.begin()

	renderer.draw_tilemap_layer(layer0)

    renderer.present()
end)