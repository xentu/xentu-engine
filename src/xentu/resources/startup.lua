-- The first lua script to run, happens before XentuGame is instantiated.

game = XentuGame()
require = game.do_script
XentuGame = nil