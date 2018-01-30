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

def inv_scale(x):
    "Translates a scalled coordinate into a pixel location"

    w = window.width
    h = window.height
    if (w < h):
        return x * 1000 / w

    return x * 1000 / h
