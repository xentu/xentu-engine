--[[
	Pong Game Example
]]
bg_color = Color.from_hex('#333333')
white = Color.from_hex('#ffffff')
blue = Color.from_hex('#0000ff')
green = Color.from_hex('#00FF00')
player1_y = 250
ball_x = 300
ball_y = 300
ball_speed_x = 3
ball_speed_y = 3
score = 0


-- the init event
game.on("init", function()
	print("Pong Game Example")

	-- create a font.
	texture2 = assets.load_texture("fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap2 = assets.load_spritemap("fonts/arial_20.json")
	font = Font(texture2, spritemap2)
	font.letter_spacing = 1
	font.line_height = 24

	renderer.set_clear_color(bg_color)
	renderer.set_text_color(green)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	-- move the ball
	ball_x = ball_x + ball_speed_x
	ball_y = ball_y + ball_speed_y

	-- check for collisions
	if (ball_y <= 0) then ball_speed_y = 3 end
	if (ball_y >= 580) then ball_speed_y = -3 end

	-- check paddle collision
	if (ball_x <= 30 and ball_y > player1_y - 10 and ball_y < player1_y + 90) then 
		ball_speed_x = 3
		score = score + 1
	end
	
	if (ball_x >= 750 and ball_y > player1_y - 10 and ball_y < player1_y + 90) then
		ball_speed_x = -3
		score = score + 1
	end

	-- check for endgame condition
	if (ball_x <= 0 or ball_x >= 780) then 
		ball_speed_x = 3
		ball_speed_y = 3
		ball_x = 300
		ball_y = 300
		score = 0
	end

	-- handle user input
	if keyboard.key_down(KB_UP) and player1_y > 8 then player1_y = player1_y - 8 end
	if keyboard.key_down(KB_DOWN) and player1_y < 492 then player1_y = player1_y + 8 end
end)


-- the draw event
game.on("draw", function()
	renderer.begin()
	-- draw line down the middle
	renderer.draw_rect(white, 399, 0, 2, 600)
	-- draw the ball
	renderer.draw_rect(blue, ball_x, ball_y, 20, 20)
	-- draw the two paddles
	renderer.draw_rect(white, 10, player1_y, 20, 100)
	renderer.draw_rect(white, 770, player1_y, 20, 100)
	-- draw the score.
	renderer.draw_text(font, "Score: " .. score, 360, 20, 100)
	-- present the image.
	renderer.present()
end)