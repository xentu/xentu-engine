function load_map(path)
    -- load the tilemap.
	map = assets.load_tilemap(path)
	-- grab some handles to layers loaded in the map.
	layers.background = map.get_layer(0)
	layers.tiles = map.get_layer(1)
	layers.collision = map.get_layer(2)
	layers.jewels = map.get_layer(3)
	layers.enemies = map.get_layer(4)
	layers.entities = map.get_layer(5)

	-- we will need the layer offset to calculate collider positions
	local offset_x, offset_y = layers.collision.get_offset()

	-- the collision layer contains a list of 2-point polygons, so lets read
	-- them and create some collision information.
	colliders = {}
	for i=0, layers.collision.object_count -1, 1 do
		local obj = layers.collision.get_object(i)
		-- get the point locations for the polyline
		local x1, y1 = obj.get_point(0)
		local x2, y2 = obj.get_point(1)

		x1 = x1 + obj.x + offset_x
		y1 = y1 + obj.y + offset_y
		x2 = x2 + obj.x + offset_x
		y2 = y2 + obj.y + offset_y

		local y_max = math.max(y1, y2)
		local collider = { left = x1, right = x2, y = y_max }

		-- add to our collider table.
		table.insert(colliders, collider)
	end
end


-- game can call this function to test if a player has hit the floor
function will_hit_floor(player)
	local px = player.x + 15
	local pw = 32
	local ph = 64
	for key, collider in pairs(colliders) do
		if px >= collider.left and px + pw < collider.right and 
		   player.y + ph > collider.y and player.y + ph < collider.y + 8 then
			return true
		end
	end
	return false
end