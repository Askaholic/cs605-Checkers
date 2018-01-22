# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window

from pyglet.gl import *
from util import scaleX, scaleY

class BoardHandlers(object):

    def __init__(self, window):
        self.window = window

    def on_draw(self):
        pass
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        draw_square(0, 0, scaleX(500), scaleY(500))

def draw_square(x, y, w, h):
    glBegin(GL_POLYGON)
    glVertex2f(x, y)
    glVertex2f(x + w, y)
    glVertex2f(x + w, y + h)
    glVertex2f(x, y + h)
    glEnd()
