audio0 = assets.load_sound("/assets/bounce1.wav")
music0 = assets.load_music("/assets/melody.ogg")

# setup variables.
renderer.set_background('#000000')
renderer.set_foreground("#FF0000")

# handle the update event.
def update(dt):
	if keyboard.key_clicked(KB_ESCAPE): game.exit()
	if keyboard.key_clicked(KB_M): audio.play_music(music0, 0)
	if keyboard.key_clicked(KB_S): audio.play_sound(audio0, -1, 0)

# handle the draw event
def draw(dt):
	renderer.clear()
	renderer.begin()
	renderer.draw_rectangle(0, 0, 100, 100)
	renderer.present()