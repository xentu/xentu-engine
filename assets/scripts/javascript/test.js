print('Hello from javascript world!');

game.create_window();

const texture0 = assets.load_texture("/images/test.png");

print("Texture was assigned the id #" + texture0);

renderer.set_clear_color('#000000');

game.on("draw", function() {
	renderer.clear();
	renderer.draw_texture(texture0, 10, 10, 100, 100);
});

game.run();