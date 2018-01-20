# board.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Draws the board onto the window


class BoardHandlers(object):

    def __init__(self, window):
        self.window = window

    def on_draw(self):
        print('drawing')
