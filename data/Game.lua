--[[
   Xentu Usercode Example
   --------------------------
   Make sure to include Game.lua as this is the main entry point.
]]



-- define some sprite infomation
sprite = {
    pos = { x=0, y=0 },
    speed = { x=2, y=0 },
    scale = { x=1, y=1 },
    rotation = 0
}



-- say hello to the world.
print("Hello from Lua world!")



-- load stuff before the game starts up.
game.on("initialized", function()
    local assets = game.assets

    assets.load_texture("texture1", "../data/Images/511-256x256.jpg")
    assets.load_sprite_map("texture1", "../data/Images/511-256x256.json")
    assets.load_audio("piano_c", "../data/Audio/Piano_Hit.mp3")
    assets.load_audio("piano_a", "../data/Audio/Piano_Hit2.mp3")

    game.log("Hello From Lua!")
end)



-- handle an event
game.on("update", function()
    if sprite.speed.x > 0 and sprite.pos.x + 1 > 700 then sprite.speed.x = -2 end
    if sprite.speed.x < 0 and sprite.pos.x - 1 < 0 then sprite.speed.x = 2 end

    sprite.pos.x = sprite.pos.x + sprite.speed.x

    if sprite.pos.x == 50 or sprite.pos.x == 250 or sprite.pos.x == 450 or sprite.pos.x == 650 then
        game.audio.play("piano_c")
    end

    if sprite.pos.x == 150 or sprite.pos.x == 350 or sprite.pos.x == 550 then
       game.audio.play("piano_a")
    end

    handle_input()
end)



-- drawing event
game.on("draw", function()
    local renderer = game.renderer

    renderer.begin()
    renderer.draw_sprite("texture1", "full", 25, 25, 50, 50)
    renderer.set_origin(50, 50)
    renderer.set_rotation(sprite.rotation)
    renderer.set_scale(sprite.scale.x, sprite.scale.y)
    renderer.draw_sprite("texture1", "top_left", sprite.pos.x + 25, sprite.pos.y + 25, 100, 100)
    renderer.present()
end)



-- do something when the player presses enter.
function handle_input()
    if game.input.key_clicked(Enums.KB_ESCAPE) or game.input.mouse_down(Enums.MOUSE_LEFT) then
        game.exit()
    elseif game.input.key_down(Enums.KB_UP) then
        y_offset = y_offset - 1
    elseif game.input.key_down(Enums.KB_DOWN) then
        y_offset = y_offset + 1
    end
end