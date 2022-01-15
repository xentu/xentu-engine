--[[
	Visual Novel Example
]]
scenes = {}


require('scene.menu.lua')


game.on('init', function()
    print("game loaded")
    game.set_scene(scenes.menu)
end)


game.on('update', function(dt)

end)


game.on('draw', function()
    
end)