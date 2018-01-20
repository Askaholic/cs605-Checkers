# util.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Helper functions for the gui

window = None


def init(w):
    global window
    window = w


def scale(x):
    "Scales based on the current window height"
    w = window.width
    h = window.height
    if (w < h):
        return x * w / 1000

    return x * h / 1000
