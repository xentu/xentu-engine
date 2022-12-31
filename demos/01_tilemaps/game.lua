print('Hello from javascript!')

-- load resources.
tex0 = assets.load_texture("/assets/sprites.png")
tilemap1 = assets.load_tilemap_tmx("/assets/tilemap01.tmx", "/assets/")

-- setup variables.
renderer.set_background('#000000')

-- handle the draw event.
game.on("draw", function(dt)
	renderer.clear();
	renderer.begin();
	renderer.draw_tile_layer(tilemap1, 0)
	renderer.draw_tile_layer(tilemap1, 1)
	renderer.draw_tile_layer(tilemap1, 2)
	renderer.present()
end)