# start: Scene Module --------------------------------------------------------

class ScenesManager:
    current=None
    current_name=''
    entries={}
    
    def on(self, name, event, cb):
        if not name in self.entries:
            self.entries[name] = {}
        self.entries[name][event] = cb
    
    def trigger(self, event, arg):
        if self.current and self.current[event]:
            self.current[event](arg)

    def select(self, name):
        if self.entries[name]:
            self.current = self.entries[name]
            self.current_name = name
    
    def remove(self, name):
        if self.entries[name]:
            self.entries[name] = None

def handle_game_update(dt):
    scenes.trigger('update', dt)

def handle_game_draw(dt):
    scenes.trigger('draw', dt)

globals()['scenes'] = ScenesManager()
globals()['handle_game_update'] = handle_game_update
globals()['handle_game_draw'] = handle_game_draw

game.on('update', 'handle_game_update')
game.on('draw', 'handle_game_draw')

# end: Scene Module ----------------------------------------------------------