# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window

from pyglet.gl import *
from pyglet.resource import image
from pyglet.sprite import Sprite
from .util import scale

class BoardHandlers(object):

    def __init__(self, window):
        self.window = window
        self.board = {}
        self.sprites = {
                'b': Sprite(img=image('black_checker.png')),
                'B': Sprite(img=image('black_king.png')),
                'r': Sprite(img=image('red_checker.png')),
                'R': Sprite(img=image('red_king.png'))
            }

    def on_draw(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.window.width // 2 - scale(400), self.window.height // 2 - scale(400), 0)
        draw_board()
        draw_pieces(self.sprites, self.board)


def draw_board():
    glColor3f(0, 0, 0)
    draw_square(0, 0, scale(800), scale(800))
    glColor3f(1, 1, 1)
    for i in range(8):
        for j in range(8):
            if (i % 2 == 0 and j % 2 != 0) or (i % 2 != 0 and j % 2 == 0):
                continue
            w = scale(100)
            h = w
            draw_square(i * w, j * h, w, h)


def draw_pieces(sprites, board):
    if not board or not hasattr(board, 'board'):
        return

    for square in board.board:
        sprite = sprites.get(square)
        if not sprite:
            continue




def draw_square(x, y, w, h):
    glBegin(GL_POLYGON)
    glVertex2f(x, y)
    glVertex2f(x + w, y)
    glVertex2f(x + w, y + h)
    glVertex2f(x, y + h)
    glEnd()
