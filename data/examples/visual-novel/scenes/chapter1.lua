--[[
	Chapter 1 Scene
]]
local scene = Scene("Chapter 1")
local data = { }


scene.on('init', function()
    print("Chapter 1 Initialized")
end)


scene.on('update', function()
    -- update the scene
end)


scene.on('draw', function()
    -- draw the scene
end)


return { handle=scene, data=data }