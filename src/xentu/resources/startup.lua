-- Xentu Startup Code
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
	if not (type(texture) == "number") then error("Invalid value for texture passed to the Sprite constructor.") end
	if not (type(x) == "number") then error("Non-numeric value for x passed to the Sprite constructor.") end
	if not (type(y) == "number") then error("Non-numeric value for y passed to the Sprite constructor.") end
	if not (type(width) == "number") then error("Non-numeric value for width passed to the Sprite constructor.") end
	if not (type(height) == "number") then error("Non-numeric value for height passed to the Sprite constructor.") end
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
	if not (type(texture) == "number") then error("Invalid value for texture passed to the Font constructor.") end
	if not (type(spritemap) == "number") then error("Invalid value for spritemap passed to the Font constructor.") end
	inst.texture = texture
	inst.spritemap = spritemap
	inst.letter_spacing = 1
	inst.line_height = 20
end)


-- A basic colour utility class.
Color = class(function(inst, r, g, b, a)
	if not (type(r) == "number") then error("Non-numeric value passed for r value when creating a Color struct instance.") end
	if not (type(g) == "number") then error("Non-numeric value passed for g value when creating a Color struct instance.") end
	if not (type(b) == "number") then error("Non-numeric value passed for b value when creating a Color struct instance.") end
	if not (type(a) == "number") then error("Non-numeric value passed for a value when creating a Color struct instance.") end
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
Color.from_hex = function(hexColor)
	local t = type(hexColor)
	if not (t == "string") then error("Color.from_hex expects a string hex value.", 2) end
	if not (string.len(hexColor) == 7) then error("Color.from_hex expects a string with 6 digits prefixed with a hash.", 2) end

	hexColor = hexColor:gsub("#","")
	local r = tonumber("0x" .. hexColor:sub(1,2)) / 255.0
	local g = tonumber("0x" .. hexColor:sub(3,4)) / 255.0
	local b = tonumber("0x" .. hexColor:sub(5,6)) / 255.0
	local a = 1
	return Color(r, g, b, a)
end


Color.from_hsl = function(h, s, l, a)
	-- thanks to https://stackoverflow.com/questions/68317097/
	if not (type(h) == "number") then error("Non-numeric value passed for h value when using Color.from_hsl.") end
	if not (type(s) == "number") then error("Non-numeric value passed for s value when using Color.from_hsl.") end
	if not (type(l) == "number") then error("Non-numeric value passed for l value when using Color.from_hsl.") end
	h = (h * 255) / 360
    s = (s * 255) / 100
    l = (l * 255) / 100

    local r, g, b;

    if s == 0 then
        r, g, b = l, l, l; -- achromatic
    else
        local function hue2rgb(p, q, t)
            if t < 0 then t = t + 1 end
            if t > 1 then t = t - 1 end
            if t < 1 / 6 then return p + (q - p) * 6 * t end
            if t < 1 / 2 then return q end
            if t < 2 / 3 then return p + (q - p) * (2 / 3 - t) * 6 end
            return p;
        end

        local q = l < 0.5 and l * (1 + s) or l + s - l * s;
        local p = 2 * l - q;
        r = hue2rgb(p, q, h + 1 / 3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1 / 3);
    end

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


Entity.from_rect = function(x, y, width, height)
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
config = game.config
require = game.require
XentuGame = nil