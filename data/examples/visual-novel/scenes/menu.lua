--[[
	Main Menu Scene
]]
local scene = Scene("Main Menu")
local data = {
    textures = { },
    sprites = { },
    menu_items = {
        { label="Start", x=241, y=312, w=318, h=36 },
        { label="Load Game", x=241, y=348, w=318, h=36 },
        { label="Options", x=241, y=384, w=318, h=36 },
        { label="Credits", x=241, y=420, w=318, h=36 },
        { label="Exit", x=241, y=456, w=318, h=36 }
    },
    menu_selected = 1
}


scene.on('init', function()
    print("Menu Scene Initialized")
    data.textures.bg = assets.load_texture("textures/menu-bg1.png")
    bgs = Sprite(data.textures.bg, 0, 0, 800, 600)
end)


scene.on('shown', function()
    print("Menu Scene Shown")
end)


scene.on('update', function(dt)
    
    if keyboard.key_clicked(KB_DOWN) then
        data.menu_selected = data.menu_selected + 1
        if data.menu_selected > 5 then data.menu_selected = 1 end
    end

    if keyboard.key_clicked(KB_UP) then
        data.menu_selected = data.menu_selected - 1
        if data.menu_selected < 1 then data.menu_selected = 5 end
    end

    locked = false
end)


scene.on('draw', function()
    renderer.clear()
    renderer.begin()

    renderer.draw_sprite(bgs)
    renderer.set_text_color(colors.white)
    for i=1,5 do
        local item = data.menu_items[i]
        if i == data.menu_selected then
            renderer.draw_rect(colors.menuitem, item.x, item.y, item.w, item.h)
        else
            renderer.draw_text(fonts.arial_20, item.label, item.x + 10, item.y + 8)
        end
    end
    renderer.present()

    renderer.begin()
    for i=1,5 do
        if i == data.menu_selected then
            local item = data.menu_items[i]
            renderer.set_text_color(colors.black)
            renderer.draw_text(fonts.arial_20, item.label, item.x + 10, item.y + 8)
        end
    end
    renderer.present()
end)


return { handle=scene, data=data }