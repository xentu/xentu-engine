--[[
	Platformer Example
]]
colors = {
	["bg"] = Color.from_hex('#333333'),
	["white"] = Color.from_hex('#ffffff')
}
textures = { }
offset = { ['x'] = 0, ['y'] = 0 }
layers = { }


-- the init event
game.on("init", function()
	-- load the tilemap.
	map = assets.load_tilemap("assets/levels/level1.tmx")
	-- grab some handles to layers loaded in the map.
	layers['background'] = map.get_layer(0)
	layers['tiles'] = map.get_layer(1)
	layers['collision'] = map.get_layer(2)
	layers['jewels'] = map.get_layer(3)
	layers['enemies'] = map.get_layer(4)
	layers['entities'] = map.get_layer(5)
	-- get the first enemy,
	enemy1 = layers.enemies.get_object(0)
	layers.jewels.visible = false

	width1, height1 = layers.jewels.get_size()
	print('Jewels Layer Size: ' .. width1 .. 'x' .. height1)

	-- setup the renderer.
	renderer.set_clear_color(colors.bg)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	-- handle user input
	if keyboard.key_down(KB_LEFT) then offset.x = offset.x + 4 end
	if keyboard.key_down(KB_RIGHT) then offset.x = offset.x - 4 end
	if keyboard.key_down(KB_UP) then offset.y = offset.y + 4 end
	if keyboard.key_down(KB_DOWN) then offset.y = offset.y - 4 end
	enemy1.x = enemy1.x - 1
end)


-- the draw event
game.on("draw", function()
	renderer.begin()
	-- draw the tile map.
	renderer.set_position(offset.x, offset.y)
	renderer.draw_tilemap_layer(layers.background)
	renderer.draw_tilemap_layer(layers.tiles)
	if layers.jewels.visible == true then
		renderer.draw_tilemap_layer(layers.jewels)
	end
	renderer.draw_tilemap_layer(layers.enemies)
	renderer.draw_tilemap_layer(layers.entities)
	-- present the image.
	renderer.present()
end)