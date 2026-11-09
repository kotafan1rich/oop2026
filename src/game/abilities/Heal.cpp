#include "game/abilities/Heal.hpp"

#include "game/statuses/Shield.hpp"

bool Heal::use(Robot& owner, World& world, std::optional<Point> target) {
    if (owner.getEnergy() < cost_ ||
        owner.getHealth() == owner.getMaxHealth()) {
        return false;
    }

    if (owner.getHealth() + healCount_ > owner.getMaxHealth()) {
        int shieldValue = owner.getHealth() + healCount_ - owner.getMaxHealth();
        owner.addPendingStatus(std::move(createAppliedStatus(shieldValue)));
    }

    owner.heal(healCount_);
    owner.expendEnergy(cost_);
    return true;
}

int Heal::energyCost() const { return cost_; }

int Heal::range() const { return range_; }

AbilityType Heal::getType() const { return AbilityType::Heal; }

std::unique_ptr<Status> Heal::createAppliedStatus(int value) {
    return std::make_unique<Shield>(value);
}

void Heal::upgrade() { healCount_ += 5; }
