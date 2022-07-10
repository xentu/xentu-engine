# -----------------------------------------------------------


class XentuGame:
	def set_screen(self, width, height, fullscreen):
		self.screen = [width, height]
		self.fullscreen = fullscreen

	def set_clear_color(self, hex_color):
		self.clear_color = hex_color

	def set_scene(this, scene):
		return 0

	def update(self, dt):
		return 0
	
	def draw(self, dt):
		return 0


# -----------------------------------------------------------


class XentuScene:
	def update(self, dt): return 0

	def draw(self, dt): return 0

	def show(self): return 0

	def hide(self): return 0


# -----------------------------------------------------------


def game_run(game):
	game.update(game, 10)
	game.draw(game, 10)
	print("game ran")