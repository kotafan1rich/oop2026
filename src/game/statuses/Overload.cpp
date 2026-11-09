#include "Overload.hpp"

StatusType Overload::getType() const { return StatusType::Overload; }

bool Overload::isExpire() const { return consumed_; }

void Overload::repeat(const Status&) {}

bool Overload::blockAbilityAttempt() {
    if (consumed_) {
        return false;
    }
    consumed_ = true;
    return true;
}
