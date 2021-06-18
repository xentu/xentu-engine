-- Used to setup some Lua only features, such as various class style structure
-- templates that can be passed to the C++ side of the engine. 


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
	inst.r = r
	inst.g = g
	inst.b = b
	inst.a = a
	inst.toHex = function()
		local hex = {
			('%02X'):format(tonumber(inst.r)),
			('%02X'):format(tonumber(inst.g)),
			('%02X'):format(tonumber(inst.b))
		}
		return table.concat(hex)
	end
	inst.toString = function()
		return inst.r .. ',' .. inst.g .. ',' .. inst.b
	end
end)


-- A extension method for creating a Colour instance from a hex colour string (aka #553221).
Color.fromHex = function(hexColor)
	hexColor = hexColor:gsub("#","")
	local r = tonumber("0x" .. hexColor:sub(1,2))
	local g = tonumber("0x" .. hexColor:sub(3,4))
	local b = tonumber("0x" .. hexColor:sub(5,6))
	return Color(r, g, b, a)
end