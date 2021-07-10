-- Used to setup some Lua only features, such as various class style structure
-- templates that can be passed to the C++ side of the engine. Also creates the
-- game global, and overrides the require function with our own smarter one.


-- A template for defining classes.
function class(base, init)
	local c = {}
	if not init and type(base) == 'function' then
		init = base
		base = nil
	elseif type(base) == 'table' then
		for i,v in pairs(base) do
			c[i] = v
		end
		c._base = base
	end
	c.__index = c
	local mt = {}
	mt.__call = function(class_tbl, ...)
		local obj = {}
		setmetatable(obj,c)
		if init then
			init(obj,...)
		else
			if base and base.init then
			base.init(obj, ...)
			end
		end
		return obj
	end
	c.init = init
	c.is_a = function(self, klass)
		local m = getmetatable(self)
		while m do
			if m == klass then return true end
			m = m._base
		end
		return false
	end
	setmetatable(c, mt)
	return c
end


-- A class that holds information about a sprite to draw graphicss.
Sprite = class(function(inst, texture, x, y, width, height)
	inst.texture = texture
	inst.x = x
	inst.y = y
	inst.width = width
	inst.height = height
	inst.spritemap = 0
	inst.region = 'full'
end)


-- A class that holds information about a font that can be used to draw text.
Font = class(function(inst, texture, spritemap) 
	inst.texture = texture
	inst.spritemap = spritemap
	inst.letter_spacing = 1
	inst.line_height = 20
end)


-- A basic colour utility class.
Color = class(function(inst, r, g, b, a)
	inst.red = r
	inst.green = g
	inst.blue = b
	inst.alpha = a
	inst.toHex = function()
		local hex = {
			('%02X'):format(tonumber(inst.red * 255)),
			('%02X'):format(tonumber(inst.green * 255)),
			('%02X'):format(tonumber(inst.blue * 255))
		}
		return table.concat(hex)
	end
	inst.toString = function()
		return (inst.red*255) .. ',' .. (inst.green*255) .. ',' .. (inst.blue*255)
	end
end)


-- Method for creating a Colour instance from a hex colour string (aka #553221).
Color.fromHex = function(hexColor)
	hexColor = hexColor:gsub("#","")
	local r = tonumber("0x" .. hexColor:sub(1,2)) / 255.0
	local g = tonumber("0x" .. hexColor:sub(3,4)) / 255.0
	local b = tonumber("0x" .. hexColor:sub(5,6)) / 255.0
	local a = 1
	return Color(r, g, b, a)
end


-- A generic class for describing a theoretical entity in the game.
Entity = class(function(inst)
	inst.x = 0
	inst.y = 0
	inst.width = 0
	inst.height = 0
	inst.color = Color.fromHex('#000000')
end)


Entity.fromRect = function(x, y, width, height)
	ent = Entity()
	ent.x = x
	ent.y = y
	ent.width = width
	ent.height = height
	return ent
end


-- Func for creating scene instances
function new_scene()
	return XentuScene()
end 


-- Wrap things up.
game = XentuGame()
require = game.require
XentuGame = nil

