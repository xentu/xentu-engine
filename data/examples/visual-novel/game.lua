--[[
	Visual Novel Example
]]
scenes = {}


game.on('init', function()
    -- load the scenes
    print("Game Path: " .. game.path)


    require('scenes/menu.lua')
    require('scenes/chapter1.lua')

    -- set the initial scene to use
    game.set_scene(scenes.menu)
end)