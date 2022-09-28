import game, assets, audio, config, const, renderer, textbox, keyboard

print("Hello from python!\n")

# load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)
text0 = assets.create_textbox(10, 10, 680, 40)
audio0 = assets.load_sound("/audio/bounce1.wav")
music0 = assets.load_music("/music/melody.ogg")

# setup colours and fonts
renderer.set_background('#444444') # set the clear color (94, 186, 125).
renderer.set_foreground("#00FFFF") # set the foreground color.
textbox.set_text(text0, font0, "Hello World") # set the text on text0.
textbox.set_color(text0, font0, "#FFFF00") # set the text to yellow.
x = 0; x_speed = 2; rot = 0
fullscreen = False

tw, th = textbox.measure_text(text0, font0, "Testing")
print("\nMeasurement: ", tw, "x", th)

# handle the update event.
def update_callback(dt):
	global x_speed, x
	if (x_speed > 0 and x + 2 > 390): x_speed = -2
	if (x_speed < 0 and x - 2 < 10):	x_speed = 2
	x += x_speed
	if (keyboard.key_clicked(const.KB_ESCAPE)): game.exit()
	if (keyboard.key_clicked(const.KB_F)):
		global fullscreen
		fullscreen = not fullscreen
		renderer.set_window_mode(fullscreen if 1 else 0)
		print(fullscreen if "window_mode: fullscreen" else "window_mode: window")
	if (keyboard.key_clicked(const.KB_M)): audio.play_music(music0, 0)
	if (keyboard.key_clicked(const.KB_S)): audio.play_sound(audio0, -1, 0)

#handle the draw event
def draw_callback(dt):
	global x_speed, x, rot
	renderer.clear()
	renderer.begin()
	renderer.set_origin(50, 50)
	renderer.set_rotation(rot)
	rot = rot + x_speed
	renderer.draw_texture(texture0, x + 50, 60, 100, 100)
	renderer.begin()
	renderer.draw_textbox(text0)
	renderer.present()

# attach events & run game.
game.on("update", "update_callback")
game.on("draw", "draw_callback")
game.run()
assets.include('/scripts/python/test2.py')