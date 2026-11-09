#include "game/abilities/Splash.hpp"

#include <functional>
#include <vector>

#include "game/statuses/Overload.hpp"
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
        bool killed = world.attackRobots(owner, robot);
        if (!killed) {
            robot.addPendingStatus(std::move(createAppliedStatus()));
        }
    }

    Player& player = world.getPlayer();
    if (owner.getTeam() != player.getTeam() &&
        owner.getPosition().dist(player.getPosition()) <= range_) {
        world.attackRobots(owner, player);
        if (!player.isDead()) {
            player.addPendingStatus(std::move(createAppliedStatus()));
        }
    }

    owner.expendEnergy(cost_);
    return true;
}

int Splash::energyCost() const { return cost_; }

int Splash::range() const { return range_; }

AbilityType Splash::getType() const { return AbilityType::Splash; }

std::unique_ptr<Status> Splash::createAppliedStatus() {
    return std::make_unique<Overload>();
}

void Splash::upgrade() { range_ += 1; }
