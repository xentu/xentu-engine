--[[
	Xentu Test Gane
	--------------------------
	A game used for testing features to add to Xentu.
]]


game.on("init", function()
	print("Hello from Lua world!")

	c = Color.from_hex("#550000")
	print("Red: " .. c.red)

	renderer.set_blend(true)
end)


game.on("update", function()
	-- do nothing for now
end)


game.on("draw", function()
	renderer.begin()
	-- do something here
	renderer.present()
end)