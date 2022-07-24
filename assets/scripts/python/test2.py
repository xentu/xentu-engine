import sys
from xentu import assets, const, game, geometry, graphics

# create a window (using settings in the config).
window = game.create_window()
#window = game.create_window_ex("My Game", const.POS_AUTO, const.POS_AUTO, 640, 480, 0)

# create a renderer (using settings in the config) linked to the window we created.
renderer = graphics.create_renderer(window)
#renderer = graphics.create_renderer(window, -1, const.RM_ACCELERATED, const.RT_IMMEDIATE)

# load a texture.
texture0 = assets.load_texture("/images/test.png")
rect0 = geometry.Rect(0, 0, 32, 32)
sprite0 = geometry.Sprite(texture0, rect0)


# define a function that is called when update logic is requested for the game.
def update_callback(dt):
	pass


# define a function that is called when the next frame is due to be drawn.
def draw_callback(dt):
	renderer.clear()
	#renderer.draw_texture(texture0, rect0)
	renderer.draw_sprite(sprite0)
	renderer.present()
	print('draw called at ' + dt)



# hook the events, and begin the game.
game.on('draw', 'draw_callback')
game.on('update', 'update_callback')
game.run()