# Test List

A guide to myself on how to test everything is working correctly (or at-least
reporting correctly).

----

## Test Checklist

Check to make sure errors are thrown correctly when:

### All
- game boots (machine code) [done 02-01-2023]
- config loads gracefully [done 02-01-2023]

### JavaScript
- events are triggered (machine code & game code) [done 02-01-2023]
- interop functions are called. [done 02-01-2023]

### Lua:
- events are triggered (machine code & game code) [done 02-01-2023]
- interop functions are called. [done 02-01-2023]

### Python:
- events are triggered (machine code & game code) [done 02-01-2023]
- interop functions are called. [done 02-01-2023]

---


## Methodology

This is how to test the above checklist. Notice that the further down the list,
the more important it is to explain errors clearly. And the higher up the list
the more critical the error becomes.

### Game Boots (machine code)
If there is an engine error, it should be explained in the console neatly before
the engine quits.


### Config Loads Gracefully
The engine should load a config gracefully, meaning if there is no config, then
load defaults. If the config is corrupt it should show an error in the console
and fail to boot.

Checks should also be done to make sure the config is sensible. For example a
window or viewport below 30x30 should be seen as invalid.


### Game Boots (game code)

Game code errors are super important as they help a developer understand coding
mistakes whilst building games. This test should make sure the errors are always
given, and that they are descriptive enough not to confuse (where possible).


### Events Triggered

Check to make sure the primary events fire correctly by using error_demo_events,
this demo throws an error when the primary events are called in the engine to
demonstrate the error message a user will see.


### Interop Functions Called

This section calls for testing methods such as asset loaders.