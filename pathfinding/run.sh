#!/bin/bash

MAP=${1:-maps/01.map}

PLAYER_RUNNER="python3 mouse.py"
python3 world.py $MAP "$PLAYER_RUNNER"
