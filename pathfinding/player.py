import sys
import math
from collections import deque

GOAL='G'
WALL='#'
BLANK=' '

UP = 0
DOWN = 1
RIGHT = 2
LEFT = 3

Position = (int, int)
DIRECTIONS = [(0,-1), (0,1), (1,0), (-1,0)]
get_adjs = lambda player: map(lambda direction: (player[0] + direction[0], player[1] + direction[1]), DIRECTIONS)


def main():
    world = dict()
    initial_position = (0,0)

    world[initial_position] = BLANK

    fila = deque()
    fila.appendleft(initial_position)

    current_positon = initial_position
    while fila:
        next_position = fila.popleft()
        print("Player", next_position, file=sys.stderr)

        if math.dist(next_position, current_positon) > 1:
            # navegar
            ...
            return 1

        print()
        sensors = list(input())
        if GOAL in sensors:
            print("Player found the goal", file=sys.stderr)
            return

        for sensor, adj_position in zip(sensors, get_adjs(current_positon)):
            world[adj_position] = sensor
            if sensor == BLANK:
                fila.appendleft(adj_position)
        current_positon = next_position

main()