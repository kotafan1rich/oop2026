#include "Burning.hpp"

#include "game/units/Robot.hpp"

Burning::Burning(int turns, int damage)
    : remainingTurns_(turns), damage_(damage) {}

StatusType Burning::getType() const { return StatusType::Burning; }

bool Burning::isExpire() const { return remainingTurns_ <= 0; }

void Burning::repeat(const Status& status) {
    if (status.getType() == getType()) {
        const auto& burning = static_cast<const Burning&>(status);
        remainingTurns_ += burning.remainingTurns_;
    }
}

void Burning::onTurnEnd(Robot& owner) {
    if (!isExpire()) {
        owner.takeDamage(damage_);
        --remainingTurns_;
    }
}
