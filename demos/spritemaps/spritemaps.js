// enable nearest neighbour filter
assets.set_interpolation(TEX_NEAREST)

// load resources.
const sprite_map1 = assets.load_sprite_map("/assets/zombie1.xsf");
var frame = 0, frame_time = 0;

// retrieve sprite info.
var fi = sprite_map.get_frame_info(sprite_map1, 'walk_right', 0);
var fc = sprite_map.get_frame_count(sprite_map1, 'walk_right');
print("Frame Time: " + fi.delay.toString());
print("Frame Count: " + fc.toString());

// handle the update event.
game.on("update", function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) exit();

	frame_time += dt;
	if (frame_time > 0.3) {
		frame++;	frame_time = 0;
		if (frame > 3) frame = 0;
	}
});

// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_sprite(sprite_map1, 'walk_right', frame, 100, 100, 100, 100);
	renderer.present();
});