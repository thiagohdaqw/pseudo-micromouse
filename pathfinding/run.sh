#!/bin/bash

MAP=${1:-maps/01.map}

PLAYER_RUNNER="python3 player.py"
python3 map.py $MAP "$PLAYER_RUNNER"
