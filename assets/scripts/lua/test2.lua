require 'xentu/sys'
require 'xentu/vfs'
require 'xentu/game'
require 'xentu/geometry'

-- setup our filesystem.
vfs.mount('/zip', './assets/test.zip')

-- create a window.
window = game.create_window("My Game", -1, -1, 640, 480, 0)

-- setup the renderer.
renderer = game.create_renderer(window, -1, XEN_RENDERER_ACCELERATED, game.XEN_RENDERER_IMMEDIATE)

-- load a texture.
texture0 = game.load_texture("/images/test.png")
rect0 = geometry.create_rect(0, 0, 32, 32)
sprite0 = geometry.create_sprite(rect0)


-- define a draw callback.
game.on('draw', function(dt)
	renderer.clear()
	renderer.draw_texture(texture0, rect0)
	-- renderer.draw_sprite(texture0, sprite0)
	renderer.present()
	print('draw called at ' + dt)
end)


game.start()