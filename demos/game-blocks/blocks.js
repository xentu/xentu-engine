assets.set_interpolation(TEX_NEAREST)

// load assets.
const t_bg = assets.load_texture('/assets/bg.png');
const t_block = assets.load_texture('/assets/block.png');



// setup variables.
const cell_size = 20;
const lookup_table = [
	[
		/*    [x, y], [x, y], [x, y], [x, y] */
		/*I*/[[0, 1], [1, 1], [2, 1], [3, 1]],
		/*J*/[[0, 0], [0, 1], [1, 1], [2, 1]],
		/*L*/[[2, 0], [0, 1], [1, 1], [2, 1]],
		/*O*/[[0, 0], [1, 0], [0, 1], [1, 1]],
		/*S*/[[1, 0], [2, 0], [0, 1], [1, 1]],
		/*T*/[[1, 0], [0, 1], [1, 1], [2, 1]],
		/*Z*/[[0, 0], [1, 0], [1, 1], [2, 1]],
	],
	[
		/*I*/[[2, 0], [2, 1], [2, 2], [2, 3]],
		/*J*/[[1, 0], [2, 0], [1, 1], [1, 2]],
		/*L*/[[1, 0], [1, 1], [1, 2], [2, 2]],
		/*O*/[[0, 0], [1, 0], [0, 1], [1, 1]],
		/*S*/[[1, 0], [1, 1], [2, 1], [2, 2]],
		/*T*/[[1, 0], [1, 1], [2, 1], [1, 2]],
		/*Z*/[[2, 0], [1, 1], [2, 1], [1, 2]],
	],
	[
		/*I*/[[0, 2], [1, 2], [2, 2], [3, 2]],
		/*J*/[[0, 1], [1, 1], [2, 1], [2, 2]],
		/*L*/[[0, 1], [1, 1], [2, 1], [0, 2]],
		/*O*/[[0, 0], [1, 0], [0, 1], [1, 1]],
		/*S*/[[1, 1], [2, 1], [0, 2], [1, 2]],
		/*T*/[[0, 1], [1, 1], [2, 1], [1, 2]],
		/*Z*/[[0, 1], [1, 1], [1, 2], [2, 2]],
	],
	[
		/*I*/[[1, 0], [1, 1], [1, 2], [1, 3]],
		/*J*/[[1, 0], [1, 1], [1, 2], [0, 2]],
		/*L*/[[0, 0], [1, 0], [1, 1], [1, 2]],
		/*O*/[[0, 0], [1, 0], [0, 1], [1, 1]],
		/*S*/[[0, 0], [0, 1], [1, 1], [1, 2]],
		/*T*/[[1, 0], [0, 1], [1, 1], [1, 2]],
		/*Z*/[[1, 0], [0, 1], [1, 1], [0, 2]],
	]
];
var stepper = assets.create_stepper(0.5, 0);
var cursor = {
	frame: 0,
	r: 0,
	x: 4, y: 0
};
var faster = false;



// setup variables.
renderer.set_background('#222222');
renderer.set_foreground("#FFFFFF");



// handle the update event.
game.on('update', function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) exit();
	if (keyboard.key_clicked(KB_LEFT)) {
		cursor.x--;
		if (cursor.x < 0) cursor.x = 0;
	}
	if (keyboard.key_clicked(KB_RIGHT)) {
		cursor.x++;
		if (cursor.x > 7) cursor.x = 7;
	}
	if (keyboard.key_clicked(KB_UP)) {
		cursor.r++; if (cursor.r > 3) cursor.r = 0;
	}
	faster = keyboard.key_down(KB_DOWN);
	stepper.delay = faster ? 0.1 : 0.4;
	if (stepper.next(dt)) {
		cursor.y += 1; 
		if (cursor.y > 18) {
			cursor.y = 0;
			cursor.r = Math.round(3 * Math.random());
			cursor.frame = Math.round(5 * Math.random());
		}
	}
});



function drawBlock(set, index, x, y) {
	const arr = lookup_table[set][index];
	const sz = cell_size;
	for (var i=0; i<arr.length; i++) {
		var tx = x + arr[i][0] * sz;
		var ty = y + arr[i][1] * sz;
		renderer.draw_texture(t_block, tx, ty, sz, sz);
	}
}



// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	renderer.draw_texture(t_bg, 0, 0, 640, 400);
	drawBlock(cursor.r, cursor.frame, 220 + (cursor.x * cell_size), (cursor.y * cell_size));
	renderer.present();
});