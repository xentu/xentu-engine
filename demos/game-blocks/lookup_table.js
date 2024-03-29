// setup lookup table for shapes and their rotations [rotation][frame].
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