#include <stdio.h>
#include <vector>
#include <iostream>
#include <array>
#include <map>
#include <cmath>
#include <time.h>

#include "motor.hpp"

using namespace std;

#ifndef FRONT_DELAY_US
#define FRONT_DELAY_US 0.5 * 1e6
#endif

typedef pair<int, int> point;
typedef point direction;

const direction UP = {-1, 0};
const direction DOWN = {1, 0};
const direction RIGHT = {1, 0};
const direction LEFT = {1, 0};
const direction NONE = {0, 0};
const array<direction, 4> DIRECTIONS = {UP, DOWN, RIGHT, LEFT};

array<point, 4> get_adjs(point p)
{
    return {point{p.first, p.second + 1}, {p.first, p.second - 1}, {p.first + 1, p.second}, {p.first - 1, p.second}};
}

int dist(point a, point b)
{
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

point add(point a, point b)
{
    return point{a.first + b.first, a.second + b.second};
}

ostream &operator<<(ostream &outs, const point &p)
{
    return outs << "(" << p.first << "," << p.second << ")";
}

class PathFinder
{
    vector<point> to_search;
    map<point, bool> world;
    point current_position = {0, 0};
    MotorDirection current_direction = MotorDirection::FRONT;

public:
    bool has_next()
    {
        return !to_search.empty();
    }

    void insert(point p, bool wall)
    {
        world.insert(make_pair(p, wall));
    }

    bool find();
    point move_to(point target);
};

bool PathFinder::find()
{
    insert(current_position, false);

    to_search.push_back(current_position);

    while (!to_search.empty())
    {
        point target = to_search.back();
        to_search.pop_back();

        int distance = dist(current_position, target);

        if (distance > 1)
            current_position = move_to(target);
        // current_position = navigate_to(world, current_position, target_position)
        else
            current_position = move_to(target);

        // sensors = list(input())

        // if GOAL in sensors:
        //     print(f"Player has found the goal at {current_position}", file=sys.stderr)
        //     return

        // for sensor, adj_position in zip(sensors, get_adjs(current_position)):
        //     if sensor == BLANK and adj_position not in world:
        //         fila.append(adj_position)
        //     world[adj_position] = sensor
    }
    return false;
}

MotorDirection get_direction(point current, point target)
{
    if (target.first < current.first)
        return MotorDirection::FRONT;
    if (target.first > current.first)
        return MotorDirection::BACK;
    if (target.second < current.second)
        return MotorDirection::LEFT;
    if (target.second > current.second)
        return MotorDirection::RIGHT;
    return MotorDirection::STOP;
}

bool try_move(MotorDirection current, MotorDirection target)
{
    double elapsed = 0;
    bool moved = motor_rotate(current, target);

    if (current == target) {
        return moved;
    }

    long int start = micros();
    long int end = micros();

    motor_move(MotorDirection::FRONT);

    while (end - start < FRONT_DELAY_US) // TODO: ADD FRONTAL COLLISION CHECK
        end = micros();
    
    motor_move(MotorDirection::STOP);
    return true;
}

point PathFinder::move_to(point target)
{
    MotorDirection target_direction = get_direction(current_position, target);

    bool moved = try_move(current_direction, target_direction);

    if (move == MotorDirection::STOP || !moved)
        return current_position;

    current_direction = target_direction;
    return target;
}

int main()
{
    PathFinder finder;

    bool found = finder.find();
    cout << "Found? " << found << "\n";

    return 0;
}
