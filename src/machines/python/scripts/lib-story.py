# start: Story Module --------------------------------------------------------

class Story:
    def __init__(self, path):
        self._text = text = assets.read_text_file(path)
        self._callbacks = {}
        self.path = path
        self.moments = json.loads(text)
        self.cursor = 0
        self.current_moment = None
    
    def find_index(self, moment_id):
        if moment_id is None: return -1
        for idx, x in enumerate(self.moments):
            if (x.id and x.id == moment_id):
                return i
        return -1

    # move the cursor to a specific moment (by id).
    def goto_moment(self, moment_id):
        index = 0
        if not moment_id is None:
            index = self.find_index(moment_id)
        if (index >= 0):
            self.goto_index(index)
    
    # goto a specific moment index.
    def goto_index(self, index):
        self.cursor = index
        self.current_moment = self.moments[index]
        # trigger prompt event.
        self.trigger('moment', self.current_moment)

    # move the cursor to the next moment.
    def next(self):
        next = self.cursor + 1;
        if (next < len(self.moments)):
            self.goto_index(next)
    
    def reset(self):
        self.moments = json.loads(self.text)
        self.trigger('reset', None)
        self.goto_index(0)

    def trigger(self, event, data):
        if event in self._callbacks:
            cb = self._callbacks[event]
            for fn in cb:
                fn(data)

    def on(self, event, cb):
        if not event in self._callbacks:
            self._callbacks[event] = [];
        self._callbacks[event].append(cb)
    
    def start(self):
        self.goto_moment(None)

globals()['Story'] = Story

def create_story(path):
    return Story(path)

assets.load_story = create_story

# end: Story Module ----------------------------------------------------------