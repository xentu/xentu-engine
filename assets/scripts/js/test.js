print('Hello from javascript!');

// load resources.
const texture0 = assets.load_texture("/images/test.png");
const font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20);
const text0 = assets.create_textbox(10, 10, 680, 40);

// setup variables.
renderer.set_background('#00FFFF'); // set the clear color.
textbox.set_text(text0, font0, "Hello World"); // set the text on text0.
var x = 0, x_speed = 2;
var fullscreen = false;

// handle the update event.
game.on('update', function(dt) {
	// print("Delta: " + dt);
	if (x_speed > 0 && x + 5 > 390) x_speed = -2;
	if (x_speed < 0 && x - 5 < 10) x_speed = 2;
	x += x_speed;

	if (keyboard.key_clicked(KB_ESCAPE)) game.exit();
	if (keyboard.key_clicked(KB_F)) {
		fullscreen = !fullscreen;
		renderer.set_window_mode(fullscreen ? 1 : 0);
		print(fullscreen ? "window_mode: fullscreen" : "window_mode: window");
	}
});

// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_texture(texture0, x, 10, 100, 100);

	//renderer.draw_sub_texture(texture0, x, 10, 100, 100, 0, 0, 50, 50);
	renderer.draw_textbox(text0);
	renderer.present();
});

// we've finished preparing, so run the game.
game.run();