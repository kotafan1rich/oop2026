#include "game/abilities/RangedAttach.hpp"

#include "game/units/Robot.hpp"
#include "game/world/World.hpp"

bool RangedAttach::use(Robot& owner, World& world,
                       std::optional<Point> target) {
    if (!target) {
        return false;
    }

    if (owner.getEnergy() < cost_) {
        return false;
    }

    if (owner.getPosition().dist(*target) > range_) {
        return false;
    }

    auto other = world.findRobotAt(*target);
    if (!other) {
        return false;
    }

    Robot& robot = other->get();

    if (owner.getTeam() == robot.getTeam()) {
        return false;
    }

    bool isDead = owner.attack(robot, damage_);
    if (isDead && &robot != &world.getPlayer()) {
        world.killRobot(robot);
        owner.gainExp(20);
    }

    owner.expendEnergy(cost_);

    return true;
}

int RangedAttach::energyCost() const { return cost_; }

int RangedAttach::range() const { return range_; }

AbilityType RangedAttach::getType() const { return AbilityType::RangedAttack; }

void RangedAttach::upgrade() { damage_ += 5; }
