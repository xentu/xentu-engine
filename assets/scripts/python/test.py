from xentu import assets, const, game, geometry
import time

print("Hello from python world!\n")

#white = game.colorFromHex('#ffffff')
game.create_window()

texture0 = assets.load_texture("/images/test.png")
rect0 = geometry.Rect(5, 10, 200, 200)

print("Texture was assigned the id #", texture0, sep='')

def draw_callback():
	#game.clear(hWnd, white)
	game.draw_texture(rect0, texture0)
	#print("Drawn")
	pass

game.on("draw", "draw_callback")
game.run()

