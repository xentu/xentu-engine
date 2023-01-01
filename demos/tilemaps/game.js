// load resources.
const tilemap1 = assets.load_tile_map_tmx("/assets/tilemap01.tmx", "/assets/");

// handle the update event.
game.on("update", function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) exit();        
});

// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_tile_layer(tilemap1, 0);
	renderer.draw_tile_layer(tilemap1, 1);
	renderer.draw_tile_layer(tilemap1, 2);
	renderer.present();
});