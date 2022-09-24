print("\nHello from lua!")

-- load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)
text0 = assets.create_textbox(10, 10, 680, 40)

-- setup variables.
renderer.set_background('#000000') -- set the clear color.
textbox.set_text(text0, font0, "Hello World") -- set the text on text0.
x = 0; x_speed = 2

-- handle the update event.
game.on("update", function(dt)
	-- print("Delta: " .. dt)
	if x_speed > 0 and x + 5 > 390 then x_speed = -2 end
	if x_speed < 0 and x - 5 < 10 then x_speed = 2 end
	x = x + x_speed
	if keyboard.key_down(KB_ESCAPE) then game.exit() end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.begin()
	renderer.clear()
	renderer.draw_texture(texture0, x, 10, 100, 100)
	-- renderer.draw_sub_texture(texture0, x, 10, 100, 100, 0, 0, 20, 20)
	renderer.draw_textbox(text0)
	renderer.present()
end)

-- we've finished preparing, so run the game.
game.run()