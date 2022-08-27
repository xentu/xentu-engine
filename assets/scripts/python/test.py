from xentu import assets, const, game, geometry
import time

print("Hello from python world!\n")

#white = game.colorFromHex('#ffffff')
game.create_window()

texture0 = assets.load_texture("/images/test.png")
print("Texture was assigned the id #", texture0, sep='')
#rect0 = geometry.create_rect(0, 0, 200, 200)

def draw_callback():
	#game.clear(hWnd, white)
	#print("Drawn")
	pass

game.on("draw", "draw_callback")
game.run()