#include "game/world/Point.hpp"

#include <cmath>

Point::Point(int x, int y) : x_(x), y_(y) {}

Point::~Point() {}

void Point::setX(int x) { x_ = x; }

void Point::setY(int y) { y_ = y; }

void Point::setPoint(int x, int y) {
    setX(x);
    setY(y);
}

double Point::dist(const Point& target) const {
    int x2 = target.getX();
    int y2 = target.getY();

    return hypot(x2 - x_, y2 - y_);
}

bool Point::operator==(const Point& other) const {
    return getX() == other.getX() && getY() == other.getY();
}

bool Point::operator!=(const Point& other) const { return !(*this == other); }

int Point::getX() const { return x_; }

int Point::getY() const { return y_; }
