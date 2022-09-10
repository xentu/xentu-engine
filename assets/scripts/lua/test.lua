


print("\nHello from lua world!")

game.create_window()

-- const white = game.colorFromHex('#ffffff')
texture0 = assets.load_texture("/images/test.png")
rect0 = geometry.create_rect(5, 10, 200, 200)

print("Texture was assigned the id #" + texture0)

game.on("draw", function(dt)
	-- game.clear(white)
	game.draw_texture(rect0, texture0)
end)

game.run()