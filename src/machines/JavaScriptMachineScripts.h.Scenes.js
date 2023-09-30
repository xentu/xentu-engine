// begin: Scene Module -------------------------------------------------------

const scenes = { current:null, current_name:'', entries: {} };
scenes.init = function() {
	game.on('update', function(dt) { scenes.trigger('update', dt) });
	game.on('draw', function(dt)   { scenes.trigger('draw', dt)   });
};
scenes.on = function(name, event, cb) {
    if (!scenes.entries[name]) {
        scenes.entries[name] = [];
    }
	scenes.entries[name][event] = cb;
};
scenes.trigger = function(event, arg) {
	if (scenes.current && scenes.current[event] instanceof Function) {
    	scenes.current[event](arg);
    }
},
scenes.select = function(name) {
	if (scenes.entries[name] !== undefined) {
		scenes.current = scenes.entries[name];
		scenes.current_name = name;
	}
},
scenes.remove = function(name) {
	if (scenes.entries[name] !== undefined) {
		if (scenes.current_name == name) {
			scenes.current = null;
			scenes.current_name = '';
		}
		delete scenes.entries[name];
	}
}

scenes.init();

// end: Scene Module ---------------------------------------------------------