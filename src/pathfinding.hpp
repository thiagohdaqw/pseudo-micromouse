#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include <tuple>
#include <queue>
using namespace std;
#include <vector>

#include "types.hpp"


class PathFinder
{
private:
    Point current;
    Direction direction;
    queue<Point> search_positions;
public:
    PathFinder(): direction(1,0) {
        search_positions.push(current);
    }
    bool continue_search();
    vector<Action> next_actions();
};

#endif // __PATHFINDING_H__


#ifndef __PATHFINDING_IMPLEMENTATION__

#include <math.h>


static const vector<Point> directions{Point(0,1), Point(1,0), Point(0, -1), Point(-1,0)};

bool PathFinder::continue_search() {
    return !search_positions.empty();
}

vector<Action> change_direction(Direction current, Direction target) {
    vector<Action> actions;
    int step = 1;

    if (t - c == 3) {
        step = -1;
    }

    while (current != target)
    {
        actions.push_back(step == 1 ? RIGHT : LEFT );
        current = (current + step) % directions.size;
    }
    
    return actions;
}

vector<Point> path_to(Point target) {
    map<pair<int, int>, pair<int, int>> visiteds;
    bfs()
}

vector<Action> PathFinder::next_actions() {
    vector<Action> actions;
    Point target_position = search_positions.pop();

    if (target_position.is_adjacent(current)) {
        current = target_position;

    }

    return actions;
}

#endif // __PATHFINDING_IMPLEMENTATION__