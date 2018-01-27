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
        self.sprites = {
                'b': make_sprite('black_checker.png'),
                'B': make_sprite('black_king.png'),
                'r': make_sprite('red_checker.png'),
                'R': make_sprite('red_king.png')
            }

    def on_draw(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.window.width // 2 - scale(400), self.window.height // 2 - scale(400), 0)
        draw_board()
        draw_pieces(self.sprites, self.window.game_board)

def make_sprite(imagestr):
    i = image(imagestr)
    i.anchor_x = i.width // 2
    i.anchor_y = i.height // 2
    return Sprite(img=i)

def draw_board():
    # glColor3f(1, 1, 1)
    glColor4f(0, 0, 0, 0.8)
    draw_outline(0, 0, scale(800), scale(800))
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

    for i, square in enumerate(board.board.board):
        sprite = sprites.get(square)
        if not sprite:
            continue

        sprite.scale = (scale(1000) / 1000) * 2

        x = 50
        y = 50

        x += ((i % 4) * 200)
        y += (700 - ((i // 4) * 100))
        if ( i // 4) % 2 == 1:
            x += 100

        sprite.set_position(scale(x), scale(y))
        sprite.draw()

def draw_outline(x, y, w, h):
    glBegin(GL_LINE_LOOP)
    gl_square(x, y, w, h)
    glEnd()

def draw_square(x, y, w, h):
    glBegin(GL_POLYGON)
    gl_square(x, y, w, h)
    glEnd()

def gl_square(x, y, w, h):
    glVertex2f(x, y)
    glVertex2f(x + w, y)
    glVertex2f(x + w, y + h)
    glVertex2f(x, y + h)
