-- start: Scene Module -------------------------------------------------------

scenes = { enabled=false, current=nil, current_name='', entries={} }
scenes.init = function()
	game.on('update', function(dt) scenes.trigger('update', dt) end)
	game.on('draw', function(dt) scenes.trigger('draw', dt) end)
end
scenes.enable = function()
	scenes.enabled = true
end
scenes.disable = function()
	scenes.disable = true
end
scenes.on = function(name, event, cb)
    if scenes.entries[name] == nil then
        scenes.entries[name] = {}
    end
	scenes.entries[name][event] = cb
end
scenes.trigger = function(event, arg)
	if scenes.enabled and scenes.current and scenes.current[event] then
    	scenes.current[event](arg)
    end
end
scenes.select = function(name)
	if scenes.current  ~= nil then
      scenes.trigger('hidden')
	end
	if scenes.entries[name] ~= nil then
		scenes.current = scenes.entries[name]
		scenes.current_name = name
		scenes.trigger('shown')
	end
end
scenes.remove = function(name)
	if scenes.entries[name] ~= nil then
		if scenes.current_name == name then
			scenes.current = nil
			scenes.current_name = ''
		end
		scenes.entries[name] = nil
	end
end

scenes.init()

-- end: Scene Module ---------------------------------------------------------