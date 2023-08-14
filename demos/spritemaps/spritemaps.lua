-- enable nearest neighbour filter
assets.set_interpolation(TEX_NEAREST)

-- load resources.
sprite_map1 = assets.load_sprite_map("/assets/zombie1.xsf")
frame = 0; frame_time = 0

-- retrieve sprite info.
fi_delay, fi_flip_x, fi_flip_y = sprite_map.get_frame_info(sprite_map1, 'walk_right', 0)
fc = sprite_map.get_frame_count(sprite_map1, 'walk_right')
print("Frame Time: " .. fi_delay)
print("Frame Count: " .. fc)

-- handle the update event.
game.on("update", function(dt)
	if (keyboard.key_clicked(KB_ESCAPE)) then game.exit() end

	frame_time = frame_time + dt
	if (frame_time > 0.3) then
		frame = frame + 1
		frame_time = 0
		if (frame > 3) then frame = 0 end
	end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.clear()
	renderer.begin()
	renderer.draw_sprite(sprite_map1, 'walk_right', frame, 100, 100, 100, 100)
	renderer.present()
end)