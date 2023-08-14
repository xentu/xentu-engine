// setup variables.
renderer.set_background('#333333');
renderer.set_foreground("#FF0000");
var x = 0, y = 0;

// handle the update event.
game.on('update', function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) game.exit();
	if (keyboard.key_down(KB_RIGHT)) x++;
	if (keyboard.key_down(KB_LEFT)) x--;
	if (keyboard.key_down(KB_DOWN)) y++;
	if (keyboard.key_down(KB_UP)) y--;

	const axis = gamepad.get_axis(0);
	if (axis.x == 1) x++;
	if (axis.x == -1) x--;
	if (axis.y == 1) y++;
	if (axis.y == -1) y--;

	if (gamepad.button_clicked(0, 0)) {
		print("Button 0 on gamepad is was clicked.");
	}
});

// handle the draw event
game.on("draw", function(dt) {
	//bot += x_speed;
	renderer.clear();
	renderer.begin();
	renderer.draw_rectangle(x, y, 100, 100);
	renderer.present();
});


game.on("gamepad_button_down", function(button) {
	print("Button was clicked: " + button);
});