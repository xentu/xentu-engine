# Module used for asking xentu to render things.
from geometry import Sprite

class Renderer:
	def __init__(self) -> None:
		pass

	def draw_sprite(texture:int, sprite:Sprite):
		pass

	def present():
		pass

def create_renderer(window:int, driver:int=-1, render_mode:int=0, render_technique:int=0) -> Renderer:
	return Renderer()