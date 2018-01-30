# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window

from pyglet.gl import *
from pyglet.resource import image
from pyglet.sprite import Sprite
from pyglet.window import mouse
from .util import scale, inv_scale

class BoardHandlers(object):

    def __init__(self, window):
        self.window = window
        self.sprites = {
                'b': make_sprite('black_checker.png'),
                'B': make_sprite('black_king.png'),
                'r': make_sprite('red_checker.png'),
                'R': make_sprite('red_king.png')
            }
        self.dragged = None

    def on_draw(self):
        glClear(GL_COLOR_BUFFER_BIT)
        glLoadIdentity()
        glTranslatef(self.window.width // 2 - scale(400), self.window.height // 2 - scale(400), 0)
        draw_board()
        draw_pieces(self.sprites, self.window.game_board, self.dragged)

    def on_mouse_press(self, x, y, buttons, modifiers):
        if not (buttons & mouse.LEFT):
            return

        scaled_x = scale(x)
        scaled_y = scale(y)

        self.dragged = {}
        self.dragged['piece'] = self.get_piece_under(x, y)
        self.dragged['x'], self.dragged['y'] = self.adjust_window_coords(x, y)

    def on_mouse_drag(self, x, y, dx, dy, buttons, modifiers):
        self.dragged['x'],self.dragged['y'] = self.adjust_window_coords(x, y)

    def on_mouse_release(self, x, y, button, modifiers):
        if self.dragged == None:
            return

        # Tell a piece to move
        move_to = self.get_piece_under(x, y)
        self.window.game_board.board.take_move(self.dragged['piece'], move_to)
        self.dragged = None

    def adjust_window_coords(self, x, y):
        rx = x - (self.window.width // 2 - scale(400))
        ry = y - (self.window.height // 2 - scale(400))
        return (rx, ry)

    def get_piece_under(self, x, y):
        rx, ry = self.adjust_window_coords(x, y)
        rx = inv_scale(rx)
        ry = inv_scale(ry)

        piece_x = (rx // 100 % 8)
        piece_y = (ry // 100)

        if (piece_x % 2 != 0 and piece_y % 2 != 0) or (piece_x % 2 == 0 and piece_y % 2 == 0):
            return None

        return (piece_x // 2 + (7 - piece_y) * 4)

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


def draw_pieces(sprites, board, dragged):
    if not board or not hasattr(board, 'board'):
        return

    for i, square in enumerate(board.board.board):
        sprite = sprites.get(square)
        if not sprite:
            continue

        if dragged and i == dragged['piece']:
            x = dragged['x']
            y = dragged['y']

            sprite.set_position(x, y)
        else:
            x = 50
            y = 50

            x += ((i % 4) * 200)
            y += (700 - ((i // 4) * 100))
            if ( i // 4) % 2 == 1:
                x += 100

            sprite.set_position(scale(x), scale(y))
        sprite.scale = (scale(1000) / 1000) * 2
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
