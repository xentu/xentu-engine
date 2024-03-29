print("\nHello from lua!")

-- load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)
text0 = assets.create_textbox(10, 10, 680, 40)
audio0 = assets.load_sound("/audio/bounce1.wav")
music0 = assets.load_music("/music/melody.ogg")
sprite_map0 = assets.load_sprite_map("/sprite_map/zombie1.xsf")

-- setup variables.
renderer.set_background('#444444') -- set the clear color.
renderer.set_foreground("#00FFFF") -- set the foreground color.
textbox.set_text(text0, font0, "Hello World") -- set the text on text0.
textbox.set_color(text0, font0, "#FFFF00") -- set the text to yellow.
x = 0; y = 0; x_speed = 2; rot = 0; alpha = 1; frame = 0; frame_time = 0
fullscreen = false

-- retrieve sprite info.
fi_delay, fi_flip_x, fi_flip_y = sprite_map.get_frame_info(sprite_map0, 'walk_right', 0)
fc = sprite_map.get_frame_count(sprite_map0, "walk_right")
print("Frame Time: " .. fi_delay)
print("Frame Count: " .. fc)

tw, th = textbox.measure_text(text0, font0, "Testing")
print("Measurement: " .. tw .. "x" .. th)

-- handle the init event
game.on('init', function()
	print('Initialized!');
end)

-- handle the update event.
game.on("update", function(dt)
	if x_speed > 0 and x + 5 > 390 then x_speed = -2 end
	if x_speed < 0 and x - 5 < 10 then x_speed = 2 end
	x = x + x_speed
	if keyboard.key_clicked(KB_ESCAPE) then exit() end
	if keyboard.key_clicked(KB_F) then
		fullscreen = not fullscreen
		renderer.set_window_mode(fullscreen and 1 or 0)
		print(fullscreen and "window_mode: fullscreen" or "window_mode: window")
	end
	if keyboard.key_clicked(KB_M) then audio.play_music(music0, 0) end
	if keyboard.key_clicked(KB_S) then audio.play_sound(audio0, -1, 0) end
	
	frame_time = frame_time + dt
	if (frame_time > 0.3) then
		frame = frame + 1
		frame_time = 0
		if (frame > 3) then frame = 0 end
	end
	
	if (mouse.button_clicked(1)) then
		mp_x, mp_y = mouse.get_position()
		print("Mouse Click! (x:" .. mp_x .. ", y:" .. mp_y .. ")")
	end
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
	renderer.draw_sprite(sprite_map0, 'walk_right', frame, 100, 100, 50, 50)
	renderer.draw_sprite(sprite_map0, 'jump_right', frame, 150, 100, 50, 50)
	renderer.draw_sprite(sprite_map0, 'walk_left', frame, 200, 100, 50, 50)
	renderer.draw_sprite(sprite_map0, 'jump_right', frame, 250, 100, 50, 50)
	renderer.present()
end)


game.on("key_down", function(key)
   print("key down " .. key)
end);