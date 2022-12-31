print('Hello from javascript!');

// load resources.
const t_sprites = assets.load_texture("/assets/sprites.png");

// setup variables.
renderer.set_background('#000000');

// handle the update event.
game.on('update', function(dt) { /* do nothing. */ });

// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_texture(t_sprites, 0, 0, 240, 208);
	renderer.present();
});