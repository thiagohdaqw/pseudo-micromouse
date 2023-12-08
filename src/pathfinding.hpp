#ifndef __PATH_H_INCLUDED__
#define __PATH_H_INCLUDED__

#include <stdio.h>
#include <time.h>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include <queue>

#include "types.hpp"
#include "motor.hpp"
#include "ultrasonic.hpp"

using namespace std;

#ifndef FRONT_DELAY_US
#define FRONT_DELAY_US 1 * 1e6
#endif
#define FREE_WAY 30
#define FIND_DELAY 1 * 1e6

const char UNKNOWN = 0;
const char FREE = 1;
const char WALL = 2;

array<point, 4> get_adjs(point p) {
    return {point{p.first, p.second + 1},
            {p.first, p.second - 1},
            {p.first + 1, p.second},
            {p.first - 1, p.second}};
}

double dist(point a, point b) {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}

point add(point a, point b) {
    return point{a.first + b.first, a.second + b.second};
}

ostream &operator<<(ostream &outs, const point &p) {
    return outs << "(" << p.first << "," << p.second << ")";
}

class PathFinder {
    vector<point> to_search;
    map<point, char> world;
    MotorDirection current_direction = MotorDirection::FRONT;

   public:
    point current_position = {0, 0};
    bool has_next() { return !to_search.empty(); }

    void insert(point p, bool wall) { world.insert(make_pair(p, wall)); }

    void find();
    point move_to(point target);
    point navigate_to(point target);
    queue<point> get_path(point target);
    void bfs(point target, map<point, point> *visiteds);
};

MotorDirection get_direction(point current, point target) {
    if (target.first < current.first) return MotorDirection::FRONT;
    if (target.first > current.first) return MotorDirection::BACK;
    if (target.second < current.second) return MotorDirection::LEFT;
    if (target.second > current.second) return MotorDirection::RIGHT;
    return MotorDirection::FRONT;
}

char get_sensor(MotorDirection direction) {
    switch (direction) {
        case MotorDirection::FRONT:
            cout << "    FRENTE " << ultrasonic_distance_cm(UltrasonicDirection::UFRONT);
            return ultrasonic_distance_cm(UltrasonicDirection::UFRONT) >
                           FREE_WAY
                       ? FREE
                       : WALL;
        case MotorDirection::LEFT:
            cout << "    ESQUERDA " << ultrasonic_distance_cm(UltrasonicDirection::ULEFT);
            return ultrasonic_distance_cm(UltrasonicDirection::ULEFT) > FREE_WAY
                       ? FREE
                       : WALL;
        case MotorDirection::RIGHT:
            cout << "    DIREITA " << ultrasonic_distance_cm(UltrasonicDirection::URIGHT);
            return ultrasonic_distance_cm(UltrasonicDirection::URIGHT) >
                           FREE_WAY
                       ? FREE
                       : WALL;
        default:
            cout << "    TRAS\n";
            return UNKNOWN;
    }
}

MotorDirection get_relative_rotate_direction(MotorDirection current,
                                      MotorDirection target) {
    switch (current) {
        case MotorDirection::FRONT:
            if (target == MotorDirection::BACK ||
                target == MotorDirection::RIGHT)
                return MotorDirection::RIGHT;
            return MotorDirection::LEFT;
        case MotorDirection::RIGHT:
            if (target == MotorDirection::LEFT ||
                target == MotorDirection::BACK)
                return MotorDirection::RIGHT;
            return MotorDirection::LEFT;
        case MotorDirection::BACK:
            if (target == MotorDirection::LEFT ||
                target == MotorDirection::FRONT)
                return MotorDirection::LEFT;
            return MotorDirection::RIGHT;
        case MotorDirection::LEFT:
            if (target == MotorDirection::RIGHT ||
                target == MotorDirection::FRONT)
                return MotorDirection::RIGHT;
            return MotorDirection::LEFT;
    }
    return MotorDirection::FRONT;
}

MotorDirection get_relative_direction(MotorDirection current,
                                      MotorDirection target) {
    if(current == target) return MotorDirection::FRONT;
    return (MotorDirection)((target - current) % 4);
}

bool check_frontal_collision() {
    return ultrasonic_distance_cm(UltrasonicDirection::UFRONT) <
           (FREE_WAY >> 1);
}

bool try_move(MotorDirection current, MotorDirection target,
              MotorDirection target_relative) {
    bool moved = motor_rotate(current, target, target_relative);

    long int start = micros();
    long int end = start;

    motor_move(MotorDirection::FRONT);

    while ((end - start < FRONT_DELAY_US) && !check_frontal_collision()) {
        end = micros();
    }
    end = micros();

    motor_stop();
    return !check_frontal_collision();
}

point PathFinder::move_to(point target) {
    MotorDirection target_direction = get_direction(current_position, target);
    MotorDirection target_relative_direction = get_relative_rotate_direction(current_direction, target_direction);

    if (current_position == target)
        return target;

    bool moved = try_move(current_direction, target_direction,
                          target_relative_direction);

    current_direction = target_direction;

    if (!moved) {
        insert(target, WALL);
        return current_position;
    }
    return target;
}

point PathFinder::navigate_to(point target) {
    queue<point> path = get_path(target);

    cout << "Navegando: " << current_position << " " << target << "\n";

    while(!path.empty()) {
        point next = path.front();
        path.pop();

        cout << "    " << next << "\n";

        if (next == current_position)
            continue;
        

        current_position = move_to(next);

        if (current_position == target)
            return target;
    }

    return current_position;
}

queue<point> PathFinder::get_path(point target) {
    map<point, point> visiteds;
    visiteds.insert(make_pair(current_position, current_position));

    bfs(target, &visiteds);

    queue<point> path;

    path.push(target);
    point next = visiteds.at(target);

    while (1)
    {
        path.push(next);
        next = visiteds.at(next);
        if (next == current_position)
            break;
    }
    return path;
}

void PathFinder::bfs(point target, map<point, point> *visiteds) {
    queue<point> queue;

    queue.push(current_position);

    while (!queue.empty()) {
        point position = queue.front();
        queue.pop();

        array<point, 4> adjs = get_adjs(position);
        for (point adj : adjs) {
            if (visiteds->count(adj) || !world.count(adj)) {
                continue;
            }
            visiteds->insert(make_pair(adj, position));
            if (adj.first == target.first && adj.second == target.second)
                return;
            if (world.at(adj) == FREE)
                queue.push(adj);
        }
    }
}

void PathFinder::find() {

    to_search.push_back(current_position);
    world.insert(make_pair(current_position, FREE));

    while (!to_search.empty()) {
        point target = to_search.back();
        to_search.pop_back();
        char sensor = world.at(target);

        if (sensor != FREE) {
            continue;
        }

        double distance = dist(current_position, target);
        cout << "c=" << current_position << "cd=" << current_direction << " t=" << target << " s=" << (int)sensor << " d=" << distance << " ts=" << to_search.size() << "\n";

        if (distance > 1) {
            current_position = navigate_to(target);
        } else
            current_position = move_to(target);

        array<point, 4> adjs = get_adjs(current_position);
        for (point adj : adjs) {
            if (world.count(adj)) {
                continue;
            }
            MotorDirection adj_direction = get_direction(current_position, adj);
            MotorDirection relative_direction = get_relative_direction(current_direction, adj_direction);
            char sensor = get_sensor(relative_direction);
            
            if (sensor == UNKNOWN) continue;

            cout << (int)sensor << " " << adj << "\n";

            world.insert(make_pair(adj, sensor));

            to_search.push_back(adj);
        }
        cout << " -- " << to_search.size() << "\n";
    }
}

#endif  // __PATH_H_INCLUDED__
