// begin: Story Module -------------------------------------------------------

assets.load_story = function(path) {
	const text = assets.read_text_file(path);
	const story = {
		 _text: text,
		 _callbacks: [],
		 path: path,
		 moments: JSON.parse(text),
		 cursor: 0,
		 current_moment: null
	}; 

	// find the index of a moment by its id, returns -1 on fail.
	story.find_index = function(moment_id) {
		 if (moment_id == null) return -1;
		 for (var i=0; i<story.moments.length; i++) {
			  if (story.moments[i].id && story.moments[i].id == moment_id) {
					return i;
			  }
		 }
		 return -1;
	};

	// move the cursor to a specific moment (by id).
	story.goto_moment = function(moment_id) {
		 var index = moment_id == null ? 0 : story.find_index(moment_id);
		 if (index >= 0) {
			  story.goto_index(index);
		 }
	}

	// goto a specific moment index.
	story.goto_index = function(index) {
		 story.cursor = index;
		 story.current_moment = story.moments[index];
		 // trigger prompt event.
		 story.trigger('moment', story.current_moment);
	}

	// move the cursor to the next moment.
	story.next = function() {
		 var next = story.cursor + 1;
		 if (next < story.moments.length) {
			  story.goto_index(next);
		 }
	}

	// completely reset the story.
	story.reset = function() {
		 story.moments = JSON.parse(text);
		 story.trigger('reset', null);
		 story.goto_index(0);
	}

	// trigger a story event.
	story.trigger = function(event, data) {
		 if (story._callbacks[event] !== undefined) {
			  const cb = story._callbacks[event];
			  for (var i=0; i<cb.length; i++) {
					cb[i](data);
			  }
		 }
	}

	// handle various story events (moment, reset)
	story.on = function(event, cb) {
		 if (story._callbacks[event] === undefined) {
			  story._callbacks[event] = [];
		 }
		 story._callbacks[event].push(cb);
	}

	story.start = function() {
		 story.goto_moment(null);
	}
	
	return story;
}

// end: Story Module ---------------------------------------------------------