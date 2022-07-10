from xentu import *
import vfs

class MyGame(XentuGame):
	def __init__(self):
		vfs.mount('/zip', './assets/test.zip')
		self.set_screen(800, 600, False)
		self.set_clear_color("#5f78988")


	def update(self, dt):
		print("update called")
	
	
	def draw(self, dt):
		print('draw called')
		

game_run(MyGame)

