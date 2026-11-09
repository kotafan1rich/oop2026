#include "game/statuses/AttackBoost.hpp"

AttackBoost::AttackBoost(int turns, int bonus)
    : remainingTurns_(turns), bonus_(bonus) {}

StatusType AttackBoost::getType() const { return StatusType::AttackBoost; }
bool AttackBoost::isExpire() const { return remainingTurns_ <= 0; }

void AttackBoost::repeat(const Status& status) {
    if (status.getType() == getType()) {
        remainingTurns_ += static_cast<const AttackBoost&>(status).remainingTurns_;
    }
}

int AttackBoost::attackBonus() const { return bonus_; }
void AttackBoost::onTurnEnd(Robot&) { --remainingTurns_; }
