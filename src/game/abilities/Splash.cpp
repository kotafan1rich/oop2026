#include "game/abilities/Splash.hpp"

#include <functional>
#include <vector>

#include "game/units/Robot.hpp"
#include "game/world/World.hpp"

bool Splash::use(Robot& owner, World& world, std::optional<Point> target) {
    if (owner.getEnergy() < cost_) {
        return false;
    }

    std::vector<std::reference_wrapper<Robot>> killed;
    for (std::unique_ptr<Robot>& robot : world.getRobots()) {
        if (&owner == robot.get()) {
            continue;
        }

        if (owner.getPosition().dist(robot->getPosition()) <= range_ &&
            owner.getTeam() != robot->getTeam()) {
            bool isDead = owner.attack(*robot);
            if (isDead) {
                owner.gainExp(20);
                killed.push_back(std::ref(*robot));
            }
        }
    }

    Player& player = world.getPlayer();
    if (owner.getTeam() != player.getTeam() &&
        owner.getPosition().dist(player.getPosition()) <= range_) {
        owner.attack(player);
    }

    for (auto& robot : killed) {
        world.killRobot(robot);
    }

    owner.expendEnergy(cost_);
    return true;
}

int Splash::energyCost() const { return cost_; }

int Splash::range() const { return range_; }

AbilityType Splash::getType() const { return AbilityType::Splash; }

void Splash::upgrade() { range_ += 1; }
