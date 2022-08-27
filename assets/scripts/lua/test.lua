print("\nHello from lua world!")

game.create_window()

game.on("draw", function()
	print("Drawn")
end)

game.run()