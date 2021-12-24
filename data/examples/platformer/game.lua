--[[
	Pong Game Example
]]
colors = {
	["bg"] = Color.from_hex('#333333'),
	["white"] = Color.from_hex('#ffffff')
}
textures = { }
offset = { ['x'] = 0, ['y'] = 0 }


-- the init event
game.on("init", function()
	map = assets.load_tilemap("assets/levels/level1.tmx")
	layer0 = map.get_layer(0)
	layer1 = map.get_layer(1)

	renderer.set_clear_color(colors.bg)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	-- handle user input
	if keyboard.key_down(KB_LEFT) then offset.x = offset.x + 2 end
	if keyboard.key_down(KB_RIGHT) then offset.x = offset.x - 2 end
	if keyboard.key_down(KB_UP) then offset.y = offset.y + 2 end
	if keyboard.key_down(KB_DOWN) then offset.y = offset.y - 2 end
end)


-- the draw event
game.on("draw", function()
	renderer.begin()
	-- draw the tile map.
	renderer.set_position(offset.x, offset.y)
	renderer.draw_tilemap_layer(layer0)
	renderer.draw_tilemap_layer(layer1)
	-- present the image.
	renderer.present()
end)