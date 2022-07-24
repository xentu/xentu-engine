from xentu import assets, const, game, geometry
import time

print("Hello from python world!\n")


def test_event_callback():
	print("test_event_callback worked!")


def test_event_callback2():
	print("test_event_callback2 worked!")


def test2(text):
	print("test2: " + text)


game.on("test_event", "test_event_callback")
game.on("test_event", "test_event_callback2")
game.trigger("test_event")


rect0 = geometry.Rect(0, 0, 32, 32)
sprite0 = geometry.Sprite(1, rect0)
print(rect0.width)


window = game.create_window()
game.run()