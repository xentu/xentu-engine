# start: Scene Module --------------------------------------------------------

class ScenesManager:
    enabled=False
    current=None
    current_name=''
    entries={}

    def enable(self):
        self.enabled=True
    def disable(self):
        self.enabled=False
    
    def on(self, name, event, cb):
        if not name in self.entries:
            self.entries[name] = {}
        self.entries[name][event] = cb
    
    def trigger(self, event, arg=None):
        if self.enabled and self.current and event in self.current:
            if (arg):
                self.current[event](arg)
            else:
                self.current[event]()

    def select(self, name):
        if self.current:
            self.trigger('hidden')
        if self.entries[name]:
            self.current = self.entries[name]
            self.current_name = name
            self.trigger('shown')
    
    def remove(self, name):
        if self.entries[name]:
            self.entries[name] = None

globals()['scenes'] = ScenesManager()

def handle_game_update(dt):
    scenes.trigger('update', dt)

def handle_game_draw(dt):
    scenes.trigger('draw', dt)

globals()['handle_game_update'] = handle_game_update
globals()['handle_game_draw'] = handle_game_draw

game.on('update', 'handle_game_update')
game.on('draw', 'handle_game_draw')

# end: Scene Module ----------------------------------------------------------