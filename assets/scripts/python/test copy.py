from xentu import assets, const, game

print("Hello from python world!\n")

game.create_window()

texture0 = assets.load_texture("/images/test.png")
rect0 = game.create_rect(5, 10, 200, 200)
white = game.create_color('#ffffff')

print("Texture was assigned the id #", texture0, sep='')

def draw_callback():
	# game.clear( white)
	game.draw_texture(rect0, texture0)
	pass

game.on("draw", "draw_callback")
game.run()

