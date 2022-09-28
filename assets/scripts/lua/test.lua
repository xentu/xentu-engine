print("\nHello from lua!")

-- load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)
text0 = assets.create_textbox(10, 10, 680, 40)
audio0 = assets.load_sound("/audio/bounce1.wav")
music0 = assets.load_music("/music/melody.ogg")

-- setup variables.
renderer.set_background('#444444') -- set the clear color.
renderer.set_foreground("#00FFFF") -- set the foreground color.
textbox.set_text(text0, font0, "Hello World") -- set the text on text0.
textbox.set_color(text0, font0, "#FFFF00") -- set the text to yellow.
x = 0; y = 0; x_speed = 2; rot = 0
fullscreen = false

tw, th = textbox.measure_text(text0, font0, "Testing")
print("Measurement: " .. tw .. "x" .. th)

-- handle the update event.
game.on("update", function(dt)
	if x_speed > 0 and x + 5 > 390 then x_speed = -2 end
	if x_speed < 0 and x - 5 < 10 then x_speed = 2 end
	x = x + x_speed
	if keyboard.key_clicked(KB_ESCAPE) then game.exit() end
	if keyboard.key_clicked(KB_F) then
		fullscreen = not fullscreen
		renderer.set_window_mode(fullscreen and 1 or 0)
		print(fullscreen and "window_mode: fullscreen" or "window_mode: window")
	end
	if keyboard.key_clicked(KB_M) then audio.play_music(music0, 0) end
	if keyboard.key_clicked(KB_S) then audio.play_sound(audio0, -1, 0) end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.clear()
	renderer.begin()
	renderer.set_origin(50, 50)
	renderer.set_rotation(rot)
	rot = rot + x_speed
	renderer.draw_texture(texture0, x + 50, y + 60, 100, 100)
	renderer.begin()
	renderer.draw_textbox(text0)
	renderer.present()
end)

-- we've finished preparing, so run the game.
game.run()
assets.include('/scripts/lua/test2.lua')