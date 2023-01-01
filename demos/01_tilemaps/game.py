print('Hello from python!')

# load resources.
tilemap1 = assets.load_tile_map_tmx("/assets/tilemap01.tmx", "/assets/")

# handle the update event.
def update_callback(dt):
	if (keyboard.key_clicked(const.KB_ESCAPE)):
		game.exit()

# handle the draw event.
def draw_callback(dt):
	renderer.clear()
	renderer.begin()
	renderer.draw_tile_layer(tilemap1, 0)
	renderer.draw_tile_layer(tilemap1, 1)
	renderer.draw_tile_layer(tilemap1, 2)
	renderer.present()


game.on("update", "update_callback")
game.on("draw", "draw_callback")