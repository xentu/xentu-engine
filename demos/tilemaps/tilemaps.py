# load resources.
tilemap1 = assets.load_tile_map_tmx("/assets/tilemap01.tmx", "/assets/")

# handle the update event.
def update(dt):
	if (keyboard.key_clicked(KB_ESCAPE)):
		game.exit()

# handle the draw event.
def draw(dt):
	renderer.clear()
	renderer.begin()
	renderer.draw_tile_layer(tilemap1, 0)
	renderer.draw_tile_layer(tilemap1, 1)
	renderer.draw_tile_layer(tilemap1, 2)
	renderer.present()