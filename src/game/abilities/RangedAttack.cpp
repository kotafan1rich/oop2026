#include "game/abilities/RangedAttack.hpp"

#include "game/statuses/Burning.hpp"
#include "game/units/Robot.hpp"
#include "game/world/World.hpp"

bool RangedAttack::use(Robot& owner, World& world,
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

    bool killed = world.attackRobots(owner, robot, damage_);
    if (!killed) {
        robot.addPendingStatus(std::move(createAppliedStatus()));
    }

    owner.expendEnergy(cost_);

    return true;
}

int RangedAttack::energyCost() const { return cost_; }

int RangedAttack::range() const { return range_; }

AbilityType RangedAttack::getType() const { return AbilityType::RangedAttack; }

void RangedAttack::addAppliedStatus(StatusConfig statusConfig) {
    appliedStatusConfig_ = statusConfig;
}

std::unique_ptr<Status> RangedAttack::createAppliedStatus() {
    return std::make_unique<Burning>(appliedStatusConfig_.turns,
                                     appliedStatusConfig_.value);
}

void RangedAttack::upgrade() { damage_ += 5; }
