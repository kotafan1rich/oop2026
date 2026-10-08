#include "game/Factory.hpp"

#include "game/Robot.hpp"

Factory::Factory(Robot spawningRobot, int height, int width)
    : spawningRobot_(spawningRobot), height_(height), width_(width) {}

Factory::~Factory() {}

void Factory::setX(int x) { point_.setX(x); }

void Factory::setY(int y) { point_.setY(y); }

int Factory::getX() const { return point_.getX(); }

int Factory::getY() const { return point_.getY(); }

Point& Factory::getPosition() { return point_; }

const Point& Factory::getPosition() const { return point_; }

void Factory::setPosition(const Point& point) {
    setX(point.getX());
    setY(point.getY());
}

void Factory::setSpawnInterval(int interval) {
    if (interval > 0) {
        spawnInterval_ = interval;
    }
}

int Factory::getSpawnInterval() const { return spawnInterval_; }

int Factory::getHeight() const { return height_; }

int Factory::getWidth() const { return width_; }

std::unique_ptr<Robot> Factory::spawn() {
    if (stepCounter_ < getSpawnInterval()) {
        return nullptr;
    }
    return std::make_unique<Robot>(spawningRobot_);
}

void Factory::onTurnEnd() {
    stepCounter_ = (stepCounter_ + 1) % (getSpawnInterval() + 1);
}