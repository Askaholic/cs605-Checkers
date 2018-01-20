# main.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Main script to start the app gui. Run this to open the appliecation.

from pyglet import app, gl
from pyglet.graphics import draw
from pyglet.text import Label
from pyglet.window import Window, key

# Global window object
window = Window(
    resizable=True
)
is_fullscreen = False

label = Label('Hello World',
              font_name='Ubuntu',
              font_size=36,
              anchor_x='center', anchor_y='center')


@window.event
def on_draw():
    window.clear()
    label.x=window.width//2
    label.y=window.height//2
    label.draw()
    # draw(2, gl.GL_QUADS,
    #     ('v3f', (10.0, 15.0, 0.0,
    #              30.0, 35.0, 0.0,
    #              40.0, 15.0, 0.0,
    #              30.0, 0.0, 0.0)))


@window.event
def on_key_press(symbol, modifiers):
    global window, is_fullscreen

    if (symbol == key.F and modifiers == key.MOD_ACCEL):
        is_fullscreen = not is_fullscreen
        window.set_fullscreen(is_fullscreen)

    # Apparently escape also exits the application
    if (symbol == key.ESCAPE and is_fullscreen):
        is_fullscreen = False
        window.set_fullscreen(is_fullscreen)


def main():

    try:
        app.run()
    except KeyboardInterrupt as e:
        print("Exiting...")
        app.exit()


if __name__ == '__main__':
    main()
