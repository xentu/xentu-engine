--[[
   Xentu Code Example
   --------------------------
   An experiment to see how we can further clean-up the design of the lua side of xentu.
]]


speed = 5


-- called when the game first loads.
game.on("init", function()
	
	-- define some colours.
	c_red = Color.fromHex('#0ee092')
	c_blue = Color.fromHex('#0eb0e0')
	
	-- create a ball entity.
	ball = Entity.fromRect(10, 10, 30, 30)
	ball.color = c_red
	ball.x_vel = speed
	ball.y_vel = speed
	
	-- create a paddle entity.
	local half_width = viewport.width / 2;
	paddle = Entity.fromRect(half_width - 50, viewport.height - 50, 100, 10)
	paddle.color = c_blue

	-- set the background.
	renderer.set_clear_color( Color.fromHex('#5f7988') )
	
    print("Hello from Lua world!")
	renderer.set_blend(true)
end)



-- the update event.
game.on("update", function()
	-- x velocity
	ball.x = ball.x + ball.x_vel
	if ball.x + ball.width >= viewport.width then ball.x_vel = -speed end
	if ball.x <= 0 then ball.x_vel = speed end

	-- y velocity (roof)
	ball.y = ball.y + ball.y_vel
	if ball.y <= 0 then ball.y_vel = speed end

	-- fail condition
	if ball.y + ball.height >= viewport.height then 
		ball.x = 10
		ball.y = 10
		ball.x_vel = speed
		ball.y_vel = speed
	end

	-- paddle hit
	if ball.y + ball.height == paddle.y and ball.x > paddle.x - ball.width and ball.x < paddle.x + paddle.width then ball.y_vel = -speed end

	-- keyboard handler
	if keyboard.key_down(KB_LEFT) then paddle.x = paddle.x - 8 end
	if keyboard.key_down(KB_RIGHT) then paddle.x = paddle.x + 8 end

end)



-- the drawing event.
game.on("draw", function()
    renderer.begin()
	-- draw the ball
	renderer.draw_rect(ball.color, ball.x, ball.y, ball.width, ball.height)
	-- draw the paddle
	renderer.draw_rect(paddle.color, paddle.x, paddle.y, paddle.width, paddle.height)
    renderer.present()
end)