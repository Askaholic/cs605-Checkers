# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window

from pyglet.gl import *
from util import scale

class BoardHandlers(object):

    def __init__(self, window):
        self.window = window

    def on_draw(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.window.width // 2 - scale(400), self.window.height // 2 - scale(400), 0)
        draw_board()

def draw_board():
    for i in range(8):
        for j in range(8):
            if (i % 2 == 0 and j % 2 != 0) or (i % 2 != 0 and j % 2 == 0):
                continue
            w = scale(100)
            h = w
            draw_square(i * w, j * h, w, h)

def draw_square(x, y, w, h):
    glBegin(GL_POLYGON)
    glVertex2f(x, y)
    glVertex2f(x + w, y)
    glVertex2f(x + w, y + h)
    glVertex2f(x, y + h)
    glEnd()
