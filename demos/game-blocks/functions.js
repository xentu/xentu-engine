// returns the minimum left value the cursor has.
function leftBounds() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	var min = 4;
	min = Math.min(min, cell[0][0]);
	min = Math.min(min, cell[1][0]);
	min = Math.min(min, cell[2][0]);
	min = Math.min(min, cell[3][0]);
	return min;
}

// returns the maximum left value the cursor has.
function rightBounds() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	var max = 0;
	max = Math.max(max, cell[0][0]);
	max = Math.max(max, cell[1][0]);
	max = Math.max(max, cell[2][0]);
	max = Math.max(max, cell[3][0]);
	return max;
}

// returns the maximum top value the cursor has.
function topBounds() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	var min = 4;
	min = Math.min(min, cell[0][1]);
	min = Math.min(min, cell[1][1]);
	min = Math.min(min, cell[2][1]);
	min = Math.min(min, cell[3][1]);
	return min;
}

// returns the maximum top value the cursor has.
function bottomBounds() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	var max = 0;
	max = Math.max(max, cell[0][1]);
	max = Math.max(max, cell[1][1]);
	max = Math.max(max, cell[2][1]);
	max = Math.max(max, cell[3][1]);
	return max;
}

// get the cursor width (without padding).
function cursorWidth() {
	const widths = [
		[4, 3, 3, 2, 3, 3, 3],
		[1, 2, 2, 2, 2, 2, 2]
	];
	return widths[cursor.rotation % 2][cursor.frame];
}

// get the cursor height (without padding).
function cursorHeight() {
	const heights = [
		[1, 2, 2, 2, 2, 2, 2],
		[4, 3, 3, 2, 3, 3, 3]
	];
	return heights[cursor.rotation % 2][cursor.frame];
}

// determine if the cursor can move left.
function canMoveLeft() {
	// for each of the 4 cell blocks, do some collision detection.
	const cell = lookup_table[cursor.rotation][cursor.frame];
	for (var c=0; c<4; c++) {
		const b_x = cursor.left + cell[c][0];
		const b_y = cursor.top + cell[c][1];
		if (b_x > 0 && b_y < field.height && placedRowHasColumn(b_y, b_x - 1)) {
			return false;	
		}
	}	
	return cursor.left > 0 - leftBounds();
}

// determine if the cursor can move right.
function canMoveRight() {
	// for each of the 4 cell blocks, do some collision detection.
	const cell = lookup_table[cursor.rotation][cursor.frame];
	for (var c=0; c<4; c++) {
		const b_x = cursor.left + cell[c][0];
		const b_y = cursor.top + cell[c][1];
		if (b_x < field.width && b_y < field.height && placedRowHasColumn(b_y, b_x + 1)) {
			return false;
		}
	}	
	return cursor.left < field.width - rightBounds() - 1;
}


function placedRowHasColumn(row, col) {
	var placed = placed_blocks[row];
	if (typeof placed == 'undefined') return false;
	for (var i=0; i<placed.length; i++) {
		if (placed[i].column == col) return true;	
	}
	return false;
}


function canPlaceCursor() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	const tb = topBounds();
	const bottom = cursor.top + tb + cursorHeight();
	
	// for each of the 4 cell blocks, do some collision detection.
	for (var c=0; c<4; c++) {
		const b_x = cursor.left + cell[c][0];
		const b_y = cursor.top + cell[c][1];
		if (b_y < field.height && placedRowHasColumn(b_y + 1, b_x)) return true;
	}

	// else just check if we hit the bottom.	
	return bottom >= field.height;
}


function placeBlock(row, _column, _color) {
	if (row < 0) return;
	placed_blocks[row].push({ column: _column, color: _color }); 
}


function placeCursor() {
	const cell = lookup_table[cursor.rotation][cursor.frame];
	const top = cursor.top;
	placeBlock(top + cell[0][1], cursor.left + cell[0][0], cursor.frame);
	placeBlock(top + cell[1][1], cursor.left + cell[1][0], cursor.frame);
	placeBlock(top + cell[2][1], cursor.left + cell[2][0], cursor.frame);
	placeBlock(top + cell[3][1], cursor.left + cell[3][0], cursor.frame);
	cursor.top = -2;
	cursor.left = 4;
	cursor.rotation = 0;
	cursor.frame = Math.round(5 * Math.random());
}


function rotateCursor() {
	cursor.rotation++;
	if (cursor.rotation > 3) {
		cursor.rotation = 0;	
	}
	const left_bounds = 0 - leftBounds();
	if (cursor.left < left_bounds) cursor.left = 0;
	const right_bounds = rightBounds();
	const width = cursorWidth();
	while (cursor.left > field.width - 1 - right_bounds) cursor.left--;
}


function nextShape() {
	cursor.top = -2;
	cursor.rotation = 0;
	cursor.frame = Math.round(5 * Math.random());
}


function drawBlock(rotation, frame, left, top) {
	const x = unit * left + field.x;
	const y = unit * top;
	var tex = colors[frame];
	const cell = lookup_table[rotation][frame];
	for (var i=0; i<cell.length; i++) {
		var tx = x + cell[i][0] * unit;
		var ty = y + cell[i][1] * unit;
		renderer.draw_texture(tex, tx, ty, unit, unit);
	}
}


function countCompleteRows() {
	var count = 0;
	for (var y=field.height; y>0; y--) {
		if (placed_blocks[y].length == field.width) {
			count++;
		}
	}
	return count;
}


function removeCompleteRows() {
	// first count how many rows we need to remove.
	var count = countCompleteRows();
	// then do that many checks.
	for (var c=0; c<4; c++) {
		for (var y=field.height; y>0; y--) {
			if (placed_blocks[y].length == 10) {
				removeRow(y);
			}
		}
	}
	return count;
}

function removeRow(row) {
	for (var y=row; y>0; y--) {
		placed_blocks[y] = placed_blocks[y - 1];
	}
}


function addScore(value) {
	score += value;
	textbox.set_text(textboxes.score_val, roboto24, String(score));
}

function addScoreLines(count) {
	lines += count;
	score += 100 * count;
	textbox.set_text(textboxes.score_val, roboto24, String(score));
	textbox.set_text(textboxes.lines_val, roboto24, String(lines));
}

function gameEnded() {
	if (typeof placed_blocks == 'undefined') return false;
	if (typeof placed_blocks[0] == 'undefined') return false;
	return placed_blocks.length > 0 && placed_blocks[0].length > 0;
}


function restartGame() {
	score = 0; lines = 0;
	for (var row=0; row<=field.height; row++) {
		placed_blocks[row] = []
	}
	textbox.set_text(textboxes.score_val, roboto24, "0");
	textbox.set_text(textboxes.lines_val, roboto24, "0");
	nextShape();
	state = states.playing;
}