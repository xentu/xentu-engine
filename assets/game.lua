require 'xentu'


game.on('init', function(self)
	vfs.mount('/zip', './assets/test.zip')
	self.set_screen(800, 600, False)
	self.set_clear_color('#5f78988')
end)


game.on('update', function(self, dt)
	print("update called")
end)


game.on('update', function(self, dt)
	print("update called")
end)


game.run()