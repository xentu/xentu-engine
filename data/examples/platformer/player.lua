function init_player()
	-- load character Turix
	player_texture = assets.load_texture("assets/textures/player.png")
	player = Sprite(player_texture, 20, 20, 64, 64)
	player.spritemap = assets.load_spritemap("assets/spritemaps/player.json")

	-- create a var to host how much time has passed since last frame change.
	player.time = 0
	player.y_velocity = 0

    -- set some variables that'll be used for jumping/falling
    player.is_jumping = true
    player.is_falling = false

	-- function to progress to next frame.
	player.next_frame = function(self)
		-- increment the frame index
		self.ani.frame = self.ani.frame + 1
		-- loop back around
		if (self.ani.frame >= self.ani.max) then self.ani.frame = 0 end
		-- change the region property
		self.region = self.ani.prefix .. self.ani.frame
	end

	-- function to set the animation that is played.
	player.set_animation = function(self, new_prefix, new_max)
		if self.ani and self.ani.prefix == new_prefix then return end
		self.ani = { prefix = new_prefix, frame = 0, max = new_max }
		self:next_frame()
	end

	-- called once per update to progress the animation.
	player.update = function(self, dt)
		self.time = self.time + dt
		if self.time > 0.2 then
			self.time = 0
			self:next_frame()
		end

        if self.is_jumping and not self.is_falling then
            --self.is_falling = true
            self.is_jumping = false
			--
        end

		if self.y_velocity > 0 and self.is_falling then
			self.is_falling = true
		end

		if self.is_falling then
			if will_hit_floor(self) then
				self.is_falling = false
				self.is_jumping = false
				self.y_velocity = 0
			else
				self:gravity()
			end
		else
			if not will_hit_floor(self) then
				self.is_falling = true
			end
		end

		local fstr = self.is_falling and "y" or "n"
		debug_text = "Velocity: " .. self.y_velocity .. ", Falling: " .. fstr .. ")"
	end

    player.jump = function(self)
        if not self.is_jumping and not self.is_falling then
            self.is_falling = false
            self.is_jumping = true
			self.y = self.y - 3
			self.y_velocity = -10
        end
    end

    player.gravity = function(self)
        self.y = self.y + self.y_velocity
		self.y_velocity = self.y_velocity + 0.32
		-- force a terminal velocity
		if (self.y_velocity < -10) then self.y_velocity = -10 end
		if (self.y_velocity > 8) then self.y_velocity = 8 end
    end
end