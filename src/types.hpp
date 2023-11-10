#ifndef __TYPE_H__
#define __TYPE_H__

#include <math.h>
#include <vector>

using namespace std;


enum Action {
    STOP,
    BACK,
    FRONT,
    LEFT,
    RIGHT,
};

enum Direction {
    D_UP,
    D_RIGHT,
    D_DOWN,
    D_LEFT,
};

class Point {
   public:
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool is_adjacent(Point a) { return (abs(x - a.x) + abs(y - a.y)) == 1; }

    bool operator==(Point const& a) const { return x == a.x && y == a.y; }
};

#endif