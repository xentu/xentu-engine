print("Hello from lua world!")


game.on("test_event", function()
	print("test_event_callback worked!")
end)

game.on("test_event", function()
	print("test_event_callback2 worked!")
end)


game.trigger("test_event")