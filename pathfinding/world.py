import sys
import subprocess
import time

DELAY = 0.1

PLAYER='P'
GOAL='G'
WALL='#'
BLANK=' '

Position = (int, int)

VELOCITY = [
    UP := (-1,0),
    DOWN := (1,0),
    RIGHT := (0,1),
    LEFT := (0,-1),
]

CONSOLE_START_X = 10
CONSOLE_START_Y = int(CONSOLE_START_X / 2)

class World:
    map: str
    width: int
    height: int
    player: Position
    goal: Position

    def __init__(self, map_path):
        self.init_map(map_path)

    def at(self, x, y):
        return self.map[self.index(x, y)]

    def index(self, x, y):
        return x*self.width + y
    
    def player_sensor(self):
        player_x, player_y = self.player
        return "".join(self.at(x + player_x, y + player_y) for x, y in VELOCITY)
    
    def apply(self, velocity):
        player_x, player_y = self.player[0] + velocity[0], self.player[1] + velocity[1]
        if self.at(player_x, player_y) == WALL:
            raise Exception(f'Invalid moviment at ({player_x},{player_y}). Found a wall')
        self.player = (player_x, player_y)

    def draw(self):
        print('\n'*50)
        for x in range(self.height):
            for y in range(self.width):
                if self.player == (x, y):
                    print(PLAYER, end='')
                else:
                    print(self.at(x, y), end='')
            print()
        print('\n'*5)

    def init_map(self, path):
        self.width = 0
        self.height = 0
        self.map = ''

        with open(path, 'r') as f:
            rows = f.read().splitlines()
            self.height = len(rows)
            self.width = max(len(row) for row in rows)
            for row_index, row in enumerate(rows):
                for col_index, col in enumerate(row):
                    if col == PLAYER:
                        self.player = (row_index, col_index)
                        self.map += BLANK
                    elif col == GOAL:
                        self.goal = (row_index, col_index)
                        self.map += GOAL
                    else:
                        self.map += col
                for _ in range(self.width - len(row)):
                    self.map += BLANK


def main(map_path, player_runner):
    world = World(map_path)

    world.draw()
    with subprocess.Popen(player_runner.split(), stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True) as proc:
        print(world.player_sensor(), file=proc.stdin, flush=True)
       
        for move in proc.stdout:
            time.sleep(DELAY)

            vel = VELOCITY[int(move.strip())]
            try:
                world.apply(vel) 
                print(world.player_sensor(), file=proc.stdin, flush=True)
                world.draw()
            except Exception as ex:
                print('ERROR:', ex, file=sys.stderr)
                exit(1)


if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <map> <player_runner>")
    exit(1)

main(sys.argv[1], sys.argv[2])
