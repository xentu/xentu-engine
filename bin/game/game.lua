-- Source code for game.lua

print("Hello from lua!")

-- assets
texture0 = assets.load_texture("images/xentu-logo.png")
angle = 0

-- variables
renderer.set_background("#444444")

-- handle the init event
game.on("init", function()
    print("Initialized!")
end)

-- handle the update event
game.on("update", function(dt)
    if keyboard.key_clicked(KB_ESCAPE) then
        game.exit()
    end
    angle = angle + 1
end)


-- handle the draw event
game.on("draw", function(dt)
    renderer.clear()
    renderer.begin()
    renderer.set_origin(50, 50)
    renderer.set_position(60, 60)
    renderer.set_rotation(angle)
    renderer.draw_texture(texture0, 100, 100, 100, 100)
    renderer.set_rotation(0)
    renderer.draw_texture(texture0, 300, 100, 100, 100)
    renderer.present()
end)