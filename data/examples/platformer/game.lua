--[[
	Platformer Example
]]


-- load other lua files.
include('player')
include('map')


-- define variables.
colors = {
	bg = Color.from_hex('#333333'),
	white = Color.from_hex('#ffffff')
}
offset = { x = 0, y = 0 }
map = nil
layers = { }
player = nil
debug_text = "loading..."


-- the init event
game.on("init", function()
	-- create a font.
	font_texture = assets.load_texture("assets/fonts/arial_12.png")
	font_spritemap = assets.load_spritemap("assets/fonts/arial_12.json")
	font = Font(font_texture, font_spritemap)

	-- load the map
	load_map("assets/levels/level1.tmx")

	-- create the player
	init_player()
	player:set_animation("walk_right", 3)

	-- setup the renderer.
	renderer.set_clear_color(colors.bg)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	-- handle user input

	player:update(dt)
	player.moving = false
	player.move_right = true

	if keyboard.key_down(KB_LEFT) then
		player.x = player.x - 4
		player:set_animation("walk_left", 3)
		player.moving = true
		player.move_right = false
	end

	if keyboard.key_down(KB_RIGHT) then
		player:set_animation("walk_right", 3)
		player.x = player.x + 4
		player.moving = true
		player.move_right = true
	end

	-- idle animation
	if not player.moving and not player.move_right then player:set_animation("idle_left", 3) end
	if not player.moving and player.move_right then player:set_animation("idle_right", 3) end

	-- set the camera relative to the player position
	offset.x = -player.x
	offset.y = -player.y

	-- jumping
	if keyboard.key_down(KB_SPACE) then player:jump() end

	-- deal with full screen toggle
	if keyboard.key_clicked(KB_ENTER) then toggle_fullscreen() end
	if keyboard.key_down(KB_ESCAPE) then game.exit() end
end)


-- the draw event
game.on("draw", function()
	renderer.clear()
	renderer.begin()
	-- draw the tile map.
	renderer.set_position(offset.x + 150, offset.y + 150)
	renderer.draw_tilemap_layer(layers.background)
	renderer.draw_tilemap_layer(layers.tiles)
	if layers.jewels.visible == true then
		renderer.draw_tilemap_layer(layers.jewels)
	end
	renderer.draw_tilemap_layer(layers.enemies)
	renderer.draw_tilemap_layer(layers.entities)

	-- draw the player
	renderer.draw_sprite(player)

	-- render debug text
	renderer.set_position(0, 0)
	renderer.draw_text(font, debug_text, 5, 6)

	-- present the image.
	renderer.present()
end)


function toggle_fullscreen()
	if game.fullscreen then
		game.set_fullscreen(false)
	else
		game.set_fullscreen(true)
	end
end