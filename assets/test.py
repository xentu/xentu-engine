import game

print("Hello from python world!\n")


def test_event_callback():
	print("test_event_callback worked!")


def test_event_callback2():
	print("test_event_callback2 worked!")


def test2(text):
	print("test2: " + text)


#vfs.read_text_file('/test.py')
game.on("test_event", "test_event_callback")
game.on("test_event", "test_event_callback2")

game.trigger("test_event")