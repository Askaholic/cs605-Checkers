# __init__.py
# Rohan Weeden
# Created: Jan. 26, 2018

# Module gui


from .board import BoardHandlers, BoardDrawHandlers
from pyglet.gl import glClearColor, glBlendFunc, glEnable, GL_BLEND, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA
from pyglet import app, gl, resource
from pyglet.graphics import draw
from pyglet.window import Window, key
from .util import init


class CheckersGame(Window):
    """docstring for Window."""
    def __init__(self, board):
        super(CheckersGame, self).__init__(resizable=True)
        self.game_board = board
        self.game_objects = []
        self.set_minimum_size(300, 300)

        resource.path = ["../assets"]
        resource.reindex()
        init(self)

        self.push_handlers(BoardDrawHandlers(self))

        self.handlers_enabled = True
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
        glEnable(GL_BLEND)
        glClearColor(112 / 255, 166 / 255, 1, 0)

    def on_key_press(self, symbol, modifiers):
        if (symbol == key.F and modifiers == key.MOD_ACCEL):
            self.set_fullscreen(not self.fullscreen)

        # Apparently escape also exits the application
        if (symbol == key.ESCAPE and self.fullscreen):
            self.set_fullscreen(False)
            return True

    def run(self):
        try:
            app.run()
        except KeyboardInterrupt as e:
            print("Exiting...")
            app.exit()

    def update(self, dt):
        if self.game_board.winner is not None and self.handlers_enabled is True:
            self.pop_handlers()
            self.handlers_enabled = False

        self.game_board.update(dt)
        for obj in self.game_objects:
            obj.update(dt)

    def add_game_object(self, obj):
        self.game_objects.append(obj);


class PlayerCheckersGame(CheckersGame):
    def __init__(self, board):
        super(PlayerCheckersGame, self).__init__(board)

        self.push_handlers(BoardHandlers(self))
