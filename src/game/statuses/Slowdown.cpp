#include "game/statuses/Slowdown.hpp"

Slowdown::Slowdown(int turns) : remainingTurns_(turns) {}

StatusType Slowdown::getType() const { return StatusType::Slowdown; }

bool Slowdown::isExpire() const { return remainingTurns_ <= 0; }

void Slowdown::repeat(const Status& status) {
    if (status.getType() == getType()) {
        const auto& slowdown = static_cast<const Slowdown&>(status);
        remainingTurns_ += slowdown.remainingTurns_;
    }
}

bool Slowdown::blocksMovement() const { return !isExpire(); }

void Slowdown::onTurnEnd(Robot&) {
    if (!isExpire()) {
        --remainingTurns_;
    }
}
