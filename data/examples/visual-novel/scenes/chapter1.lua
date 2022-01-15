--[[
	Chapter 1 Scene
]]
local scene = new_scene()


scene.on('init', function()
    print("Chapter 1 Initialized")
end)


scene.on('update', function()
    -- update the scene
end)


scene.on('draw', function()
    -- draw the scene
end)


return scene