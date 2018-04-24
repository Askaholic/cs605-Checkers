# skynet.py
# Rohan Weeden
# Created: April 17, 2018
# Does web requests to interface with the skyent server

import requests
import json

url = "http://skynet.cs.uaf.edu/"

def info_game(name):
    resp = requests.post(
        url,
        params={"info_game": "true"},
        data=json.dumps({"name": name})
    )
    resp.raise_for_status()
    return json.loads(resp.text)

def play_turn(name, board):
    board.replace(" ", "_");
    resp = requests.post(
        url,
        params={"play_game": "true"},
        data=json.dumps({
            "name": name,
            "board": board
        })
    )
