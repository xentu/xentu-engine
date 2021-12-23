--[[
	Flappy Bird Example
]]

colors = { }
textures = { }
sprites = {
	["pipe_down"] = {},
	["pipe_up"] = {}
}

pipe_count = 50
world_x = 0
space_between_pipes = 200
time_elapsed = 0
pipe_index = 0
suspended = false
suspended_message = ""


-- the init event
game.on("init", function()
	print("Flappy Bird Clone")

	-- setup colours
	colors.bg = Color.from_hex('#333333')
	colors.black = Color.from_hex('#000000')
	colors.white = Color.from_hex('#ffffff')
	colors.blue = Color.from_hex('#0000ff')
	colors.green = Color.from_hex('#00FF00')


	-- create a font.
	texture2 = assets.load_texture("fonts/arial_20.png", TX_RGBA, TX_CLAMP_TO_BORDER)
	spritemap2 = assets.load_spritemap("fonts/arial_20.json")
	font = Font(texture2, spritemap2)
	font.letter_spacing = 1
	font.line_height = 24
	

	-- load some textures
	textures.bird = assets.load_texture("textures/bird.png", TX_RGBA, TX_REPEAT)
	textures.pipe_up = assets.load_texture("textures/pipe-up.png", TX_RGBA, TX_REPEAT)
	textures.pipe_down = assets.load_texture("textures/pipe-down.png", TX_RGBA, TX_REPEAT)


	-- create the bird sprite
	bird = Sprite(textures.bird, 0, 0, 48, 48)
	bird.x = 0
	bird.y = 250
	bird.acc = 0


	-- create sprites for the pipes
	for i=1, pipe_count do

		sprites.pipe_down[i] = Sprite(textures.pipe_down, 0, 0, 32, 300)
		sprites.pipe_down[i].x = i * space_between_pipes
		sprites.pipe_down[i].y = -200 + math.floor(math.random() * 200)

		sprites.pipe_up[i] = Sprite(textures.pipe_up, 0, 0, 32, 300)
		sprites.pipe_up[i].x = i * space_between_pipes
		sprites.pipe_up[i].y = sprites.pipe_down[i].y + 500
	end

	
	-- setup the renderer
	renderer.set_clear_color(colors.bg)
	renderer.set_text_color(colors.green)
	renderer.set_blend(true)
end)


-- the update event.
game.on("update", function(dt)
	if keyboard.key_down(KB_SPACE) then 
		time_elapsed = 125.0
		if (suspended == true) then suspended = false end
	end
	
	if (time_elapsed < 125.0 or suspended == true) then return end
	time_elapsed = time_elapsed + dt

	-- check win condition
	if (bird.y < viewport.height - bird.height) then
		bird.y = bird.y + 2
		bird.y = bird.y - bird.acc
	else
		suspended = true
		suspended_message = "You Won!"
		return
	end

	if bird.acc > 0 then bird.acc = bird.acc - 1 end
	if keyboard.key_down(KB_SPACE) then
		bird.acc = 5
		bird.y = bird.y - 1
	end

	if world_x % space_between_pipes == 0 then
		pipe_index = 1 - math.floor(world_x / space_between_pipes)
		if (pipe_index > 1 and pipe_index <= pipe_count) then
			pipe_ceiling = sprites.pipe_down[pipe_index - 1].y + 305
			pipe_floor = pipe_ceiling + 200
			if (bird.y < pipe_ceiling) then
				print("Hit ceiling")
				restart_game()
			end
			if (bird.y > pipe_floor) then
				print("Hit floor")
				restart_game()
			end
		end
	end

	world_x = world_x - 2
end)


-- the draw event
game.on("draw", function()
	renderer.begin()
	renderer.draw_sprite(bird)

	renderer.set_position(world_x, 0)
	for i=1, pipe_count do
		renderer.draw_sprite( sprites.pipe_down[i] )
		renderer.draw_sprite( sprites.pipe_up[i] )
	end

	renderer.set_text_color(colors.black)
	renderer.draw_text(font, "Score: " .. pipe_index, 21, 21, 100)
	renderer.set_text_color(colors.green)
	renderer.draw_text(font, "Score: " .. pipe_index, 20, 20, 100)

	if (time_elapsed < 125.0 and suspended == false) then
		renderer.set_text_color(colors.black)
		renderer.draw_text(font, "Press Space To Start!", 22, 42, 200)
		renderer.set_text_color(colors.white)
		renderer.draw_text(font, "Press Space To Start!", 20, 40, 200)
	end

	if (suspended == true) then
		renderer.set_text_color(colors.black)
		renderer.draw_text(font, suspended_message, 22, 42, 200)
		renderer.set_text_color(colors.white)
		renderer.draw_text(font, suspended_message, 20, 40, 200)
	end

	renderer.present()
end)


function restart_game()
	time_elapsed = 0
	bird.x = 0
	bird.y = 250
	world_x = 0
end