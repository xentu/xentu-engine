print("\nHello from lua!")

game.create_window()

-- load resources.
texture0 = assets.load_texture("/images/test.png")
font0 = assets.load_font("/fonts/Roboto-Regular.ttf", 20)

print("\nHello from lua2!")

-- we've finished preparing, so run the game.
game.run()
print("\nHello from lua3!")