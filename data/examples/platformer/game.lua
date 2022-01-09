--[[
	Platformer Example
]]
colors = {
	bg = Color.from_hex('#333333'),
	white = Color.from_hex('#ffffff')
}
textures = { }
offset = { x = 0, y = 0 }
layers = { }
ani_acc = 0
player = nil


function init_player()
	-- load character Turix
	player_texture = assets.load_texture("assets/textures/player.png")
	player = Sprite(player_texture, 20, 20, 64, 64)
	player.spritemap = assets.load_spritemap("assets/spritemaps/player.json")
	player.ani_next = function()
		-- increment the frame index
		player.ani.frame = player.ani.frame + 1
		-- loop back around
		if (player.ani.frame >= player.ani.max) then player.ani.frame = 0 end
		-- change the region property
		player.region = player.ani.prefix .. player.ani.frame
	end
	player.ani_set = function(new_prefix, new_max)
		player.ani = { prefix = new_prefix, frame = 0, max = new_max }
	end
end


-- the init event
game.on("init", function()
	-- load the tilemap.
	map = assets.load_tilemap("assets/levels/level1.tmx")
	-- grab some handles to layers loaded in the map.
	layers.background = map.get_layer(0)
	layers.tiles = map.get_layer(1)
	layers.collision = map.get_layer(2)
	layers.jewels = map.get_layer(3)
	layers.enemies = map.get_layer(4)
	layers.entities = map.get_layer(5)

	-- create the player
	init_player()
	player.ani_set("walk_right", 3)

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

	if keyboard.key_down(KB_A) then player.ani_set("walk_left", 3) end
	if keyboard.key_down(KB_D) then player.ani_set("walk_right", 3) end
	if keyboard.key_down(KB_SPACE) then player.ani_set("fall", 2) end

	ani_acc = ani_acc + dt
	if ani_acc > 1 then
		  ani_acc = 0
		player.ani_next()
	end
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

	-- draw the player
	renderer.draw_sprite(player)

	-- present the image.
	renderer.present()
end)