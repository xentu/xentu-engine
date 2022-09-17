print('Hello from javascript!');

// load resources.
const texture0 = assets.load_texture("/images/test.png");
const font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20);
const text0 = assets.create_textbox(10, 10, 680, 40);

// setup variables.
renderer.set_background('#000000'); // set the clear color.
textbox.set_text(text0, font0, "Hello World"); // set the text on text0.
var x = 0, x_speed = 5;

// handle the update event.
game.on('update', function() {
	if (x_speed > 0 && x + 5 > 690) x_speed = -5;
	if (x_speed < 0 && x - 5 < 10) x_speed = 5;
	x += x_speed;
});

// handle the draw event
game.on("draw", function() {
	renderer.begin();
	renderer.draw_texture(texture0, x, 10, 100, 100);
	renderer.draw_textbox(text0);
	renderer.present();
});

// we've finished preparing, so run the game.
game.run();