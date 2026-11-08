#include "game/abilities/Splash.hpp"

#include <functional>
#include <vector>

#include "game/units/Robot.hpp"
#include "game/world/World.hpp"

bool Splash::use(Robot& owner, World& world, std::optional<Point> target) {
    if (owner.getEnergy() < cost_) {
        return false;
    }

    std::vector<std::reference_wrapper<Robot>> targets;

    for (const auto& robot : world.getRobots()) {
        if (robot.get() != &owner && owner.getTeam() != robot->getTeam() &&
            owner.getPosition().dist(robot->getPosition()) <= range_) {
            targets.push_back(std::ref(*robot));
        }
    }

    for (Robot& robot : targets) {
        world.attackRobots(owner, robot);
    }

    Player& player = world.getPlayer();
    if (owner.getTeam() != player.getTeam() &&
        owner.getPosition().dist(player.getPosition()) <= range_) {
        world.attackRobots(owner, player);
    }

    owner.expendEnergy(cost_);
    return true;
}

int Splash::energyCost() const { return cost_; }

int Splash::range() const { return range_; }

AbilityType Splash::getType() const { return AbilityType::Splash; }

void Splash::upgrade() { range_ += 1; }
