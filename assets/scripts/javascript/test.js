print('Hello from javascript world!');

game.create_window();

game.on("draw", function(dt) {
	print("Drawn");
});

game.run();