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
DIRECTIONS = [(-1,0), (1,0), (0,1), (0,-1)]
get_adjs = lambda player: map(lambda direction: (player[0] + direction[0], player[1] + direction[1]), DIRECTIONS)


def get_direction(current_position, target_position):
    if target_position[0] < current_position[0]:
        return UP
    if target_position[0] > current_position[0]:
        return DOWN
    if target_position[1] < current_position[1]:
        return LEFT
    if target_position[1] > current_position[1]:
        return RIGHT
    return None


def move_to(current_position, target_position):
    move = get_direction(current_position, target_position)

    if move is None:
        return current_position
    
    print(move, flush=True)
    return target_position


def bfs(world, current_position, target_position, visiteds):
    queue = deque()

    queue.append(current_position)

    while queue:
        position = queue.popleft()

        for prox in get_adjs(position):
            if prox in visiteds or prox not in world:
                continue

            visiteds[prox] = position
            if prox == target_position:
                return
            if world[prox] != WALL:
                queue.append(prox)


def get_path(world, current_position, target_position):
    visiteds = dict()
    visiteds[current_position] = None
    bfs(world, current_position, target_position, visiteds)

    path = [target_position]
    prox = visiteds[target_position]

    while True:
        path.append(prox)
        prox = visiteds[prox]
        if prox is None:
            return path[::-1]


def navigate_to(world, current_position, target_position):
    path = get_path(world, current_position, target_position)

    for next_position in path[1:]:
        current_position = move_to(current_position, next_position)

        if current_position != target_position:
            input()

    return target_position


def main():
    world = dict()
    initial_position = (0,0)

    world[initial_position] = BLANK

    fila = deque()
    fila.append(initial_position)

    current_position = initial_position

    while fila:
        target_position = fila.pop()
        target_distance = math.dist(target_position, current_position)

        if target_distance > 1:
            current_position = navigate_to(world, current_position, target_position)
        else:
            current_position = move_to(current_position, target_position)    

        sensors = list(input())

        if GOAL in sensors:
            print(f"Player has found the goal at {current_position}", file=sys.stderr)
            return

        for sensor, adj_position in zip(sensors, get_adjs(current_position)):
            if sensor == BLANK and adj_position not in world:
                fila.append(adj_position)
            world[adj_position] = sensor

main()