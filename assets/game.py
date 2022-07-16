from xentu import *
import vfs
import game


vfs.mount('/zip', './assets/test.zip')
#game.set_screen(800, 600, False)
#game.set_clear_color("#5f78988")


def update(dt):
	print("update called")
	
	
def draw(dt):
	print('draw called')


game.on('update', update)
game.on('draw', draw)
#game.run(0, 0, 800, 600, False)