print("\nHello from lua!")

-- load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)
text0 = assets.create_textbox(10, 10, 680, 40)

-- setup variables.
renderer.set_background('#444444') -- set the clear color.
textbox.set_text(text0, font0, "Hello World") -- set the text on text0.
--textbox.set_color(text0, '#ff0000')
x = 0; x_speed = 2; rot = 0
fullscreen = false

-- handle the update event.
game.on("update", function(dt)
	-- print("Delta: " .. dt)
	if x_speed > 0 and x + 5 > 390 then x_speed = -2 end
	if x_speed < 0 and x - 5 < 10 then x_speed = 2 end
	x = x + x_speed
	if keyboard.key_clicked(KB_ESCAPE) then game.exit() end
	if keyboard.key_clicked(KB_F) then
		fullscreen = not fullscreen
		renderer.set_window_mode(fullscreen and 1 or 0)
		print(fullscreen and "window_mode: fullscreen" or "window_mode: window")
	end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.clear()
	renderer.begin()
	renderer.set_origin(50, 50)
	renderer.set_rotation(rot)
	rot = rot + x_speed
	renderer.draw_texture(texture0, x + 50, 60, 100, 100)
	renderer.begin()
	renderer.draw_textbox(text0)
	renderer.present()
end)

-- we've finished preparing, so run the game.
game.run()