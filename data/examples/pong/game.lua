--[[
	Pong Game Example
]]
colors = {
   bg = Color.from_hex('#333333'),
   black = Color.from_hex('#000000'),
   white = Color.from_hex('#ffffff'),
   blue = Color.from_hex('#0000ff'),
   green = Color.from_hex('#00FF00')
}
sounds = {}
player = { y = 250 }
ball = { x = 300, y = 300, speed_x = 3, speed_y = 3 }
score = 0


-- the init event.
game.on("init", function()
	-- create a font.
	texture2 = assets.load_texture("fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap2 = assets.load_spritemap("fonts/arial_20.json")
	font = Font(texture2, spritemap2)

	-- load sounds.
	sounds.bounce1 = assets.load_audio("sounds/bounce1.mp3")
	sounds.bounce2 = assets.load_audio("sounds/bounce2.mp3")
	sounds.fail = assets.load_audio("sounds/fail.mp3")
	
	-- set some rendering variables.
	renderer.set_clear_color(colors.bg)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	-- move the ball.
	ball.x = ball.x + ball.speed_x
	ball.y = ball.y + ball.speed_y

	-- check for collisions.
	if (ball.y <= 0) then
		ball.speed_y = 3
		audio.play(sounds.bounce1)
	end
	if (ball.y >= 580) then
		ball.speed_y = -3
		audio.play(sounds.bounce1)
	end

	-- check paddle collision.
	if (ball.x <= 30 and ball.y > player.y - 10 and ball.y < player.y + 90) then 
		ball.speed_x = 3
		score = score + 1
		audio.play(sounds.bounce2)
	end
	
	if (ball.x >= 750 and ball.y > player.y - 10 and ball.y < player.y + 90) then
		ball.speed_x = -3
		score = score + 1
		audio.play(sounds.bounce2)
	end

	-- check for endgame condition.
	if (ball.x <= 0 or ball.x >= 780) then 
		ball.speed_x = 3
		ball.speed_y = 3
		ball.x = 300
		ball.y = 300
		score = 0
		audio.play(sounds.fail)
	end

	-- handle user input.
	if keyboard.key_down(KB_UP) and player.y > 8 then player.y = player.y - 8 end
	if keyboard.key_down(KB_DOWN) and player.y < 492 then player.y = player.y + 8 end
end)


-- the draw event
game.on("draw", function()
	renderer.begin()
	-- draw line down the middle.
	renderer.draw_rect(colors.white, 399, 0, 2, 600)
	-- draw the ball.
	renderer.draw_rect(colors.blue, ball.x, ball.y, 20, 20)
	-- draw the two paddles.
	renderer.draw_rect(colors.white, 10, player.y, 20, 100)
	renderer.draw_rect(colors.white, 770, player.y, 20, 100)
	-- draw the score.
	renderer.set_text_color(colors.black)
	renderer.draw_text(font, "Score: " .. score, 361, 21, 100)
	renderer.set_text_color(colors.green)
	renderer.draw_text(font, "Score: " .. score, 360, 20, 100)
	-- present the image.
	renderer.present()
end)