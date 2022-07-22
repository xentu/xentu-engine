# A module for holding various engine constant values.


# coordinate constants.
X_AUTO = -1 # window x coord auto
Y_AUTO = -1 # window y coord auto


# renderer mode
RM_SOFTWARE = 0 # software rendering
RM_ACCELERATED = 1 # hardware accelerated rendering


# rendering technique
RT_IMMEDIATE = 0 # forward rendering, draw things immediately
RT_DEFERRED = 1 # deferred rendering using begin() and end()