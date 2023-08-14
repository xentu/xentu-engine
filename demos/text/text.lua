-- load in our fonts.
roboto16 = assets.load_font("/assets/Roboto-Regular.ttf", 16)
roboto24 = assets.load_font("/assets/Roboto-Regular.ttf", 24)
roboto20 = assets.load_font("/assets/Roboto-Regular.ttf", 20)

-- setup default colours.
renderer.set_background('#444444')
renderer.set_foreground("#CCCCCC")

-- create 1st textbox and measure the text on line0.
line1 = "This is the Roboto font in size 16 (light gray)"
textbox1 = assets.create_textbox(10, 10, 680, 40)
textbox.set_text(textbox1, roboto16, line1)
size1_w, size1_h = textbox.measure_text(textbox1, roboto16, line1)

-- create 2nd textbox.
line2 = "This is the Roboto font in size 24 (magenta)"
textbox2 = assets.create_textbox(10, 40, 680, 40)
textbox.set_text(textbox2, roboto24, line2)
textbox.set_color(textbox2, roboto24, "#FF00FF") -- give it a different colour.

-- create 3rd textbox.
textbox3 = assets.create_textbox(10, 80, 680, 40)
textbox.set_text(textbox3, roboto20, "The first line measures " .. size1_w .. " x " .. size1_h .. " px")
textbox.set_color(textbox3, roboto20, "#00FFFF")

-- create 4th textbox.
textbox4 = assets.create_textbox(10, 120, 680, 40)
stepper = assets.create_stepper(1, 0)
textbox.set_text(textbox4, roboto20, "This is a counter (1s delay): 0")

-- handle the update event.
game.on("update", function(dt)
	if (keyboard.key_clicked(KB_ESCAPE)) then game.exit() end
	if (stepper.next(dt)) then
		stepper.value = stepper.value + 1
		textbox.set_text(textbox4, roboto20, "This is a counter (1s delay): " .. stepper.value)
	end
end)

-- handle the draw event
game.on("draw", function(dt)
	renderer.clear()
	renderer.begin()
	renderer.draw_textbox(textbox1)
	renderer.draw_textbox(textbox2)
	renderer.draw_textbox(textbox3)
	renderer.draw_textbox(textbox4)
	renderer.present()
end)