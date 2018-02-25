# CS605 AI Checkers
CS605 Checkers playing AI using a minimax tree search and simple neural network.

Authors:
  - Jacob McKenna
  - Rohan Weeden

## Installation

1. Install `pip` requirements:
    - `pip install -r requirements.txt` (Preferably to a virtualenv)
2. Build and install the C++ extensions
    - Make sure you have the python dev dependencies installed. On ubuntu run
    `sudo apt-get install python3-dev`
    - `python3 setup.py install` (To the same virtualenv as before)

## Start the GUI
Run `python3 src/main.py`

## Running unit tests
Run `python3 test/test.py`

There are also some unit tests which require `catch.hpp`. To build these first
download `catch.hpp` and place it in the `test/` directory. Then run the makefile
from the root directory with:
  - `make`

Binaries will appear in `build/`. Run the unit tests with `build/test`.

There is also a test program for playing AI against each other. You can run it
with `build/game` to play a full game or `build/game true` to step through each
turn.
