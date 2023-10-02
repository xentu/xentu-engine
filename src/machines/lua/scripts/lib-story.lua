-- start: Story Module -------------------------------------------------------

assets.load_story = function(path)
	text = assets.read_text_file(path)
	story = {
		 _text = text,
		 _callbacks = {},
		 path = path,
		 moments = json.parse(text),
		 cursor = 0,
		 current_moment = nil
	}

	-- find the index of a moment by its id, returns 0 on fail.
	story.find_index = function(moment_id)
		 if (moment_id == null) then return 0 end
		 for i=1,story.moments.length do
			  if story.moments[i].id and story.moments[i].id == moment_id then
					return i
			  end
		 end
		 return 0
	end

	-- move the cursor to a specific moment (by id).
	story.goto_moment = function(moment_id)
		 index = moment_id == nil and 1 or story.find_index(moment_id)
		 if index >= 0 then
			  story.goto_index(index)
		 end
	end

	-- goto a specific moment index.
	story.goto_index = function(index)
		 story.cursor = index
		 story.current_moment = story.moments[index]
		 -- trigger prompt event.
		 story.trigger('moment', story.current_moment)
	end

	-- move the cursor to the next moment.
	story.next = function()
		 next = story.cursor + 1
		 if story.moments[next] ~= nil then
			  story.goto_index(next)
		 end
	end

	-- completely reset the story.
	story.reset = function()
		 story.moments = json.parse(text)
		 story.trigger('reset', null)
		 story.goto_index(1)
	end

	-- trigger a story event.
	story.trigger = function(event, data)
		 if story._callbacks[event] ~= nil then
			  cb = story._callbacks[event]
			  for i,v in ipairs(cb) do
					cb[i](data)
			  end
		 end
	end

	-- handle various story events (moment, reset)
	story.on = function(event, cb)
		 if not story._callbacks[event] ~= nil then
			  story._callbacks[event] = {}
		 end
		 table.insert(story._callbacks[event], cb)
	end

	story.start = function()
		 story.goto_moment(null)
	end
	
	return story
end

-- end: Scene Module ---------------------------------------------------------