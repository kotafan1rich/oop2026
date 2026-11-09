#include "Shield.hpp"

#include <algorithm>

Shield::Shield(int amount) : remainingShield_(amount) {}

StatusType Shield::getType() const { return StatusType::Shield; }

bool Shield::isExpire() const { return remainingShield_ <= 0; }

void Shield::repeat(const Status& status) {
    if (status.getType() == getType()) {
        const auto& shield = static_cast<const Shield&>(status);
        remainingShield_ += shield.remainingShield_;
    }
}

int Shield::absorbDamage(int damage) {
    if (damage <= 0 || isExpire()) {
        return damage;
    }
    int absorbed = std::min(damage, remainingShield_);
    remainingShield_ -= absorbed;
    return damage - absorbed;
}
