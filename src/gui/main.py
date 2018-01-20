# main.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Main script to start the app gui. Run this to open the appliecation.

from pyglet import app, gl
from pyglet.graphics import draw
from pyglet.text import Label
from pyglet.window import Window

# Global window object
window = Window()

label = Label('Hello World',
              font_name='Ubuntu',
              font_size=36,
              x=window.width//2, y=window.height//2,
              anchor_x='center', anchor_y='center')


@window.event
def on_draw():
    window.clear()
    label.draw()
    # draw(2, gl.GL_QUADS,
    #     ('v3f', (10.0, 15.0, 0.0,
    #              30.0, 35.0, 0.0,
    #              40.0, 15.0, 0.0,
    #              30.0, 0.0, 0.0)))

def main():

    try:
        app.run()
    except KeyboardInterrupt as e:
        print("Exiting...")
        app.exit()


if __name__ == '__main__':
    main()
