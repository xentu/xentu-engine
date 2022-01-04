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

	-- object point test.
	local collision_obj_count = layers.collision.object_count;
	print('Collision layer has ' .. collision_obj_count .. ' object(s) and is named ' .. layers.collision.name)
	local collision_obj1 = layers.collision.get_object(0)
	local collision_obj1_point_count = collision_obj1.point_count
	print('First collision object has ' .. collision_obj1_point_count .. ' point(s)')
	local co1p0_x, co1p0_y = collision_obj1.get_point(0)
	local co1p1_x, co1p1_y = collision_obj1.get_point(1)

	co1p0_x = co1p0_x + collision_obj1.x
	co1p1_x = co1p1_x + collision_obj1.x
	co1p0_y = co1p0_y + collision_obj1.y
	co1p1_y = co1p1_y + collision_obj1.y

	print('First collision object point0 is at ' .. co1p0_x .. 'x' .. co1p0_y)
	print('First collision object point1 is at ' .. co1p1_x .. 'x' .. co1p1_y)

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