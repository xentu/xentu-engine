import sys
from xentu import assets, const, game, geometry, graphics

# create a window.
window = game.create_window("My Game", const.POS_AUTO, const.POS_AUTO, 640, 480, 0)
# create a renderer that draws onto 
renderer = graphics.create_renderer(window, -1, const.RM_ACCELERATED, const.RT_IMMEDIATE)

# load a texture.
texture0 = assets.load_texture("/images/test.png")
rect0 = geometry.Rect(0, 0, 32, 32)
sprite0 = geometry.Sprite(texture0, rect0)

# define a draw function, xentu will call this automatically.
def draw_callback(dt):
	renderer.clear()
	#renderer.draw_texture(texture0, rect0)
	renderer.draw_sprite(sprite0)
	renderer.present()
	print('draw called at ' + dt)

# hook the draw function.
game.on('draw', 'draw_callback')
game.run()