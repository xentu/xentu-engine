// setup engine options.
renderer.set_background('#222222');
renderer.set_foreground("#FFFFFF");


// load assets.
const textures = {
	bg: assets.load_texture('/assets/bg.png'),
	block: assets.load_texture('/assets/block.png')
};
const colors = [
	assets.load_texture('/assets/block_cyan.png')   /* I - #00ffff */,
	assets.load_texture('/assets/block_blue.png')   /* J - #0000ff */,
	assets.load_texture('/assets/block_orange.png') /* L - #ff7f00 */,
	assets.load_texture('/assets/block_yellow.png') /* O - #ffff00 */,
	assets.load_texture('/assets/block_green.png')  /* S - #00ff00 */,
	assets.load_texture('/assets/block_purple.png') /* T - #800080 */,
	assets.load_texture('/assets/block_red.png')    /* Z - #ff0000 */
];
const roboto24 = assets.load_font("/assets/Roboto-Regular.ttf", 24);
const textboxes = {
	gameover1: assets.create_textbox(260, 30),
	gameover2: assets.create_textbox(260, 30),
	gameover3: assets.create_textbox(260, 30),
	score_head: assets.create_textbox(200, 26),
	score_val: assets.create_textbox(200, 26),
	lines_head: assets.create_textbox(200, 26),
	lines_val: assets.create_textbox(200, 26),
};


// choose a base unit for our blocks.
const unit = 20;
// setup some variables for keeping score.
var lines = 0, score = 0;
// setup our cursor (the piece that moves).
var cursor = {
	frame: 0,     // the block type
	rotation: 0,  // 90 degree angles, 0-3 (or 0,90,180,270)
	left: 4,	  // the starting unit x position
	top: -2       // the starting unit y position
};
// setup our play field.
var field = { 
	x: unit * 11, // the drawn x coordinate.
	width: 10,    // the number of units wide.
	height: 20    // the number of units (including 0)
};

// setup steppers so that we can animate things.
var key_stepper = assets.create_stepper(0.1, 0);
var left_down = false, right_down = false;
var fall_stepper = assets.create_stepper(0.5, 0);
var fall_stepper_faster = false;

// create something to store the game state.
const states = { playing:0, check_rows:1, ended:2 };
var state = states.playing;


// set the text on our textboxes.
textbox.set_text(textboxes.gameover1, roboto24, "GAME OVER");
textbox.set_text(textboxes.gameover2, roboto24, "SCORE");
textbox.set_text(textboxes.gameover3, roboto24, "00000");
textbox.set_text(textboxes.score_head, roboto24, "SCORE");
textbox.set_text(textboxes.score_val, roboto24, "0");
textbox.set_text(textboxes.lines_head, roboto24, "LINES");
textbox.set_text(textboxes.lines_val, roboto24, "0");


// set the text colors.
textbox.set_color(textboxes.gameover1, roboto24, "#00FFFF");


// include dependencies.
include("/lookup_table.js");
include("/functions.js");


// build the placed blocks array, each block looks like this { left: 0, color: tex_id }.
var placed_blocks = [];
for (var row=0; row<=field.height; row++) {
	placed_blocks[row] = []
}
// we use a variable to fade out the blocks that worked.
var complete_opacity = 1; rot = 0;





// handle the update event.
game.on('update', function(dt) {
	if (keyboard.key_clicked(KB_ESCAPE)) exit();
	
	if (state == states.ended) {
		if (keyboard.key_clicked(KB_RETURN)) {
			restartGame();
		}
	}
	else if (state == states.playing) {
		if (gameEnded()) {
			state = states.ended;
			textbox.set_text(textboxes.gameover3, roboto24, String(score));
			return;
		}
		
		if (keyboard.key_clicked(KB_UP)) rotateCursor();
		left_down = keyboard.key_down(KB_LEFT);
		right_down = keyboard.key_down(KB_RIGHT);
		if (key_stepper.next(dt)) {
			if (left_down && canMoveLeft()) cursor.left--;
			if (right_down && canMoveRight()) cursor.left++;
		}
		
		fall_stepper.delay = keyboard.key_down(KB_DOWN) ? 0.1 : 0.4;
		if (fall_stepper.next(dt)) {
			if (canPlaceCursor()) {
				placeCursor();
				if (countCompleteRows() > 0) {
					state = states.check_rows;
					complete_opacity = 1;
					rot = 0;
				}
			}
			cursor.top += 1;
			if (keyboard.key_down(KB_DOWN)) {
				addScore(1);
			}
		}
	}
	else if (state == states.check_rows) {
		complete_opacity -= 0.03;
		rot += 2;
		if (complete_opacity <= 0) {
			state = states.playing;
			var count = removeCompleteRows();
			addScoreLines(count);
		}
	}
});


// handle the draw event
game.on("draw", function(dt) {
	renderer.clear();
	renderer.begin();
	// draw the background.
	renderer.draw_texture(textures.bg, 0, 0, 640, 400);
	// draw the cursor.
	drawBlock(cursor.rotation, cursor.frame, cursor.left, cursor.top);
	// draw the already placed cells (non complete rows).
	for (var y=0; y<field.height; y++) {
		if (placed_blocks[y].length == 10) continue;
		for (var i=0; i<placed_blocks[y].length; i++) {
			const block = placed_blocks[y][i];
			var tx = block.column * unit + field.x;
			var ty = y * unit;
			renderer.draw_texture(colors[block.color], tx, ty, unit, unit);
		}
	}
	
	if (state == states.check_rows) {
		renderer.set_alpha(complete_opacity);
		renderer.set_origin(10, 10);
		renderer.set_position(10, 10);
		renderer.set_rotation(rot);
	}
	
	// draw the already placed cells (complete rows only).
	for (var y=0; y<field.height; y++) {
		if (placed_blocks[y].length != 10) continue;
		for (var i=0; i<placed_blocks[y].length; i++) {
			const block = placed_blocks[y][i];
			var tx = block.column * unit + field.x;
			var ty = y * unit;
			renderer.draw_texture(colors[block.color], tx, ty, unit, unit);
		}
	}
	
	renderer.begin();
	renderer.draw_textbox(textboxes.score_head, 460, 16);
	renderer.draw_textbox(textboxes.score_val, 460, 36);
	renderer.draw_textbox(textboxes.lines_head, 460, 76);
	renderer.draw_textbox(textboxes.lines_val, 460, 96);
	
	if (state == states.ended) {
		renderer.begin();
		renderer.set_foreground("#333333");
		renderer.draw_rectangle(180, 90, 280, 190);
		renderer.set_foreground("#555555");
		renderer.draw_rectangle(190, 100, 260, 170);
		renderer.draw_textbox(textboxes.gameover1, 260, 120);
		renderer.draw_textbox(textboxes.gameover2, 290, 170);
		renderer.draw_textbox(textboxes.gameover3, 290, 220);
	}
	
	renderer.present();
});


