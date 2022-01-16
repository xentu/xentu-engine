--[[
	Main Menu Scene
]]
local scene = Scene("Main Menu")
local data = {
    textures = { },
    sprites = { }
}


scene.on('init', function()
    print("Menu Scene Initialized")
    data.textures.bg = assets.load_texture("textures/menu-bg1.png")
    data.sprites.bg = Sprite(data.textures.bg, 0, 0, 800, 600)
end)


scene.on('shown', function()
    print("Menu Scene Shown")
end)


scene.on('update', function()
    -- update the scene
end)


scene.on('draw', function()
    renderer.begin()
    renderer.draw_sprite(data.sprites.bg)
    renderer.present()
end)


return { handle=scene, data=data }