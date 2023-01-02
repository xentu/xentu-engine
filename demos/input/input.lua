-- setup variables.
renderer.set_background('#333333')
renderer.set_foreground("#FF0000")
x = 0; y = 0

-- handle the update event.
game.on('update', function(dt)
	if keyboard.key_clicked(KB_ESCAPE) then exit() end
	if keyboard.key_down(KB_RIGHT) then x = x + 1 end
	if keyboard.key_down(KB_LEFT) then x = x - 1 end
	if keyboard.key_down(KB_DOWN) then y = y + 1 end
	if keyboard.key_down(KB_UP) then y = y - 1 end

	ax, ay = gamepad.get_axis(0)
	if ax ==  1 then x = x + 1 end
	if ax == -1 then x = x - 1 end
	if ay ==  1 then y = y + 1 end
	if ay == -1 then y = y - 1 end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.clear()
	renderer.begin()
	renderer.draw_rectangle(x, y, 100, 100)
	renderer.present()
end)

game.on("gamepad_button_down", function(button)
	print("Button was clicked: " + button);
end)