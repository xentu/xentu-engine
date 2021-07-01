--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]



-- called when the game first loads.
game.on("init", function()
	-- define some colours.
	c_red = Color.fromHex('#880000')
	c_blue = Color.fromHex('#000088')

	-- create a ball entity.
	ball = Entity.fromRect(10, 10, 30, 30)
	ball.color = c_red
	ball.x_vel = 2
	ball.y_vel = 2

	-- create a paddle entity.
	--paddle = Entity.fromRect(100, 100, 10, 10)
	--paddle.color = c_blue

	-- color class tests.
	local c = Color.fromHex('#CCCCCC')
	renderer.set_clear_color(c)
	
    print("Hello from Lua world!")
	renderer.set_blend(true)
end)



-- the update event.
game.on("update", function()
	-- x velocity
	ball.x = ball.x + ball.x_vel
	if ball.x + ball.width >= viewport.width then ball.x_vel = -2 end
	if ball.x <= 0 then ball.x_vel = 2 end
	-- y velocity
	ball.y = ball.y + ball.y_vel
	if ball.y + ball.height >= viewport.height then ball.y_vel = -2 end
	if ball.y <= 0 then ball.y_vel = 2 end

	-- print('Set pos y=' .. ball.x .. ',y=' .. ball.y)
end)



-- the drawing event.
game.on("draw", function()
    renderer.begin()
	renderer.draw_rect(ball.color, ball.x, ball.y, ball.width, ball.height)
    renderer.present()
end)