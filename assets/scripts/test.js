print('Hello from javascript!');

// load resources.
const texture0 = assets.load_texture("/images/test.png");
const font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20);
const text0 = assets.create_textbox(10, 10, 680, 40);
const audio0 = assets.load_sound("/audio/bounce1.wav");
const music0 = assets.load_music("/music/melody.ogg");

// setup variables.
renderer.set_background('#444444'); // set the clear color.
renderer.set_foreground("#00FFFF");
textbox.set_text(text0, font0, "Hello World"); // set the text on text0.
textbox.set_color(text0, font0, "#FFFF00"); // set the text to yellow.
var x = 0, x_speed = 2, rot = 0;
var fullscreen = false;

var size = textbox.measure_text(text0, font0, "Testing");
print("Measurement: " + size.w + "x" + size.h);

// handle the update event.
game.on('update', function(dt) {
	// print("Delta: " + dt);
	if (x_speed > 0 && x + 2 > 390) x_speed = -2;
	if (x_speed < 0 && x - 2 < 10) x_speed = 2;
	x += x_speed;
	if (keyboard.key_clicked(KB_ESCAPE)) game.exit();
	if (keyboard.key_clicked(KB_F)) {
		fullscreen = !fullscreen;
		renderer.set_window_mode(fullscreen ? 1 : 0);
		print(fullscreen ? "window_mode: fullscreen" : "window_mode: window");
	}
	if (keyboard.key_clicked(KB_M)) audio.play_music(music0, 0);
	if (keyboard.key_clicked(KB_S)) audio.play_sound(audio0, -1, 0);
});

// handle the draw event
game.on("draw", function(dt) {
	rot += x_speed;
	renderer.clear();
	renderer.begin();
	renderer.set_origin(50, 50);
	renderer.set_rotation(rot);
	renderer.draw_texture(texture0, x + 50, 60, 100, 100);
	renderer.begin();
	renderer.draw_textbox(text0);
	renderer.present();
});


// we've finished preparing, so run the game.
game.run();