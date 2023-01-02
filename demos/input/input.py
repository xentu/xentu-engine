# setup variables.
renderer.set_background('#333333')
renderer.set_foreground("#FF0000")
x = 0; y = 0

# handle the update event.
def update(dt):
	global x, y
	if keyboard.key_clicked(KB_ESCAPE): exit()
	if keyboard.key_down(KB_RIGHT): x = x + 1
	if keyboard.key_down(KB_LEFT): x = x - 1
	if keyboard.key_down(KB_DOWN): y = y + 1
	if keyboard.key_down(KB_UP): y = y - 1

	ax, ay = gamepad.get_axis(0)
	if ax ==  1: x = x + 1
	if ax == -1: x = x - 1
	if ay ==  1: y = y + 1
	if ay == -1: y = y - 1

# handle the draw event
def draw(dt):
	renderer.clear()
	renderer.begin()
	renderer.draw_rectangle(x, y, 100, 100)
	renderer.present()

# handle button press
def handle_button_press(button):
	print("Button was clicked: " + button)

game.on("gamepad_button_down", 'handle_button_press')