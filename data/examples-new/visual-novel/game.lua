--[[
	Visual Novel Example
]]
scenes = {}


game.on('init', function()
    -- load the scenes
    scenes.menu = include('scenes.menu')
    scenes.chapter1 = include('scenes/chapter1')

    -- set the initial scene to use
    game.set_scene(scenes.menu)
end)