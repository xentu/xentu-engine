print('Hello from javascript world!');
print('Hello from javascript world2!');

game_create_window();

//const white = game.colorFromHex('#ffffff')
const texture0 = assets_load_texture("/images/test.png");
//const rect0 = geometry_create_rect(5, 10, 200, 200);

print("Texture was assigned the id #" + texture0);

game_on("draw", function() {
	renderer_clear();
	renderer_draw_texture(texture0, 10, 10, 100, 100);
});

game_run();