print('Hello from javascript!');


game.create_window();


const texture0 = assets.load_texture("/images/test.png");
var x = 0, x_speed = 5;
renderer.set_clear_color('#000000');


const lang = config.get_str("game", "language");
print("Language = " + lang);


game.on('update', function() {
	if (x_speed > 0 && x + 5 > 700) x_speed = -5;
	if (x_speed < 0 && x - 5 < 0) x_speed = 5;
	x += x_speed;
});


game.on("draw", function() {
	renderer.clear();
	renderer.draw_texture(texture0, x, 10, 100, 100);
});


game.run();