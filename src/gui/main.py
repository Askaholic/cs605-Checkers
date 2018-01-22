# main.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Main script to start the app gui. Run this to open the appliecation.

from board import BoardHandlers
from pyglet.gl import *
from pyglet import app, gl, resource
from pyglet.graphics import draw
from pyglet.text import Label
from pyglet.window import Window, key
from util import init


# Global window object
window = Window(
    resizable=True
)

#
# label = Label('Hello World',
#               font_name='Ubuntu',
#               font_size=36,
#               anchor_x='center', anchor_y='center')


def on_draw():
    window.clear()
    # label.x=window.width//2
    # label.y=window.height//2
    # label.draw()


@window.event
def on_key_press(symbol, modifiers):
    global window

    if (symbol == key.F and modifiers == key.MOD_ACCEL):
        window.set_fullscreen(not window.fullscreen)

    # Apparently escape also exits the application
    if (symbol == key.ESCAPE and window.fullscreen):
        window.set_fullscreen(False)
        return True


def main():
    global window

    resource.path = ["./resources"]
    resource.reindex()

    window.push_handlers(
        on_draw,
        BoardHandlers(window)
    )

    init(window)

    try:
        app.run()
    except KeyboardInterrupt as e:
        print("Exiting...")
        app.exit()


if __name__ == '__main__':
    main()
