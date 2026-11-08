#include "game/abilities/Heal.hpp"

bool Heal::use(Robot& owner, World& world, std::optional<Point> target) {
    if (owner.getEnergy() < cost_ ||
        owner.getHealth() == owner.getMaxHealth()) {
        return false;
    }
    owner.heal(healCount_);
    owner.expendEnergy(cost_);
    return true;
}

int Heal::energyCost() const { return cost_; }

int Heal::range() const { return range_; }

AbilityType Heal::getType() const { return AbilityType::Heal; }
