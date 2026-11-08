#include "game/abilities/Teleport.hpp"

#include "game/units/Robot.hpp"
#include "game/world/World.hpp"

bool Teleport::use(Robot& owner, World& world, std::optional<Point> target) {
    if (!target) {
        return false;
    }

    Cell& targetCell = world.getCell(*target);
    if (owner.getPosition().dist(*target) > range_ || targetCell.isTaken() ||
        !targetCell.isWalkable()) {
        return false;
    }

    if (owner.getEnergy() < cost_) {
        return false;
    }

    Point& point = *target;
    int dx = point.getX() - owner.getPosition().getX();
    int dy = point.getY() - owner.getPosition().getY();

    MoveResult result = world.moveRobot(owner, dx, dy);

    if (result == MoveResult::Blocked) {
        return false;
    }
    owner.expendEnergy(cost_);
    return true;
}

int Teleport::range() const { return range_; }

int Teleport::energyCost() const { return cost_; }

AbilityType Teleport::getType() const { return AbilityType::Teleport; }
