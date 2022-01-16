--[[
	Main Menu Scene
]]
local scene = Scene("Main Menu")


scene.on('init', function()
    print("Menu Scene Initialized")
end)


scene.on('shown', function()
    print("Menu Scene Shown")
end)


scene.on('update', function()
    -- update the scene
end)


scene.on('draw', function()
    -- draw the scene
end)


return scene