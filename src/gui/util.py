# util.py
# Rohan Weeden
# Created: Jan. 19, 2018

# Helper functions for the gui

window = None


def init(w):
    global window
    window = w


def scaleX(x):
    "Scales based on the current window width"
    return x * window.height / 1000



def scaleY(y):
    "Scales based on the current window width"
    return y * window.height / 1000
