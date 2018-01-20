# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window

from pyglet import gl
from pyglet.graphics import draw


class BoardHandlers(object):

    def __init__(self, window):
        self.window = window

    def on_draw(self):
        pass
        # draw(1, gl.GL_QUADS,
        # ('v2i', (100, 100,
        #          150, 100,
        #          150, 150,
        #          100, 150)))
