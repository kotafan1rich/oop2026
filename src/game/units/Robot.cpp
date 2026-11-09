#include "game/units/Robot.hpp"

#include <algorithm>
#include <string>

#include "game/world/Point.hpp"

Robot::Robot(int speed) { setSpeed(speed); }

Robot::~Robot() = default;

void Robot::setX(int x) { point_.setX(x); }

void Robot::setY(int y) { point_.setY(y); }

void Robot::setPosition(const Point& point) {
    setX(point.getX());
    setY(point.getY());
}

void Robot::setHealth(int health) {
    health_ = std::clamp(health, 0, maxHealth_);
}

int Robot::getHealth() const { return health_; }

void Robot::setMaxHealth(int maxHealth) {
    if (maxHealth <= 0) {
        return;
    }

    maxHealth_ = maxHealth;
    health_ = std::min(health_, maxHealth_);
}

int Robot::getMaxHealth() const { return maxHealth_; }

void Robot::setAttack(int attack) {
    if (attack >= 0) {
        attack_ = attack;
    }
}

int Robot::getAttack() const { return attack_; }

void Robot::setSpeed(int speed) {
    if (speed > 0) {
        speed_ = speed;
    }
}

int Robot::getSpeed() const { return speed_; }

void Robot::setEnergy(int energy) {
    energy_ = std::clamp(energy, 0, maxEnergy_);
}

int Robot::getEnergy() const { return energy_; }

void Robot::setMaxEnergy(int maxEnergy) {
    if (maxEnergy <= 0) {
        return;
    }

    maxEnergy_ = maxEnergy;
    energy_ = std::min(energy_, maxEnergy_);
}

int Robot::getMaxEnergy() const { return maxEnergy_; }

void Robot::expendEnergy(int value) {
    if (value >= 0) {
        setEnergy(energy_ - value);
    }
}

void Robot::restoreEnergy(int value) {
    if (value >= 0) {
        setEnergy(energy_ + value);
    }
}

int Robot::gainExp(int value) { return 0; }

void Robot::addAbility(std::unique_ptr<Ability> ability) {
    for (auto& a : abilities_) {
        if (ability->getType() == a->getType()) {
            return;
        }
    }
    abilities_.push_back(std::move(ability));
}

const std::vector<std::unique_ptr<Ability>>& Robot::getAllAbilities() const {
    return abilities_;
}

std::vector<AbilityType> Robot::getAllAbilityTypes() const {
    std::vector<AbilityType> result;
    for (auto& ability : abilities_) {
        result.push_back(ability->getType());
    }
    return result;
}

std::optional<std::reference_wrapper<Ability>> Robot::getAbility(
    AbilityType abilityType) const {
    for (auto& ability : abilities_) {
        if (ability->getType() == abilityType) {
            return *ability;
        }
    }
    return std::nullopt;
}

bool Robot::upgradeAbility(AbilityType abilityType) {
    auto abilityOpt = getAbility(abilityType);
    if (abilityOpt) {
        abilityOpt->get().upgrade();
        return true;
    }
    return false;
}

void Robot::addStatus(std::unique_ptr<Status> status) {
    for (auto& currentStatus : statuses_) {
        if (status->getType() == currentStatus->getType()) {
            currentStatus->repeat(*status);
            return;
        }
    }
    statuses_.push_back(std::move(status));
}

void Robot::addPendingStatus(std::unique_ptr<Status> status) {
    pendingStatuses_.push_back(std::move(status));
}

const std::vector<std::unique_ptr<Status>>& Robot::getAllStatuses() const {
    return statuses_;
}

std::optional<std::reference_wrapper<Status>> Robot::getStatus(
    StatusType statusType) const {
    for (auto& currentStatus : statuses_) {
        if (currentStatus->getType() == statusType) {
            return *currentStatus;
        }
    }
    return std::nullopt;
}

void Robot::clearExpiredStatuses() {
    std::erase_if(statuses_,
                  [](const auto& status) { return status->isExpire(); });
}

int Robot::getX() const { return point_.getX(); }

int Robot::getY() const { return point_.getY(); }

Point& Robot::getPosition() { return point_; }

const Point& Robot::getPosition() const { return point_; }

void Robot::move(int dx, int dy) {
    point_.setX(point_.getX() + dx);
    point_.setY(point_.getY() + dy);
}

void Robot::onTurnEnd() {
    for (auto& status : getAllStatuses()) {
        status->onTurnEnd(*this);
    }

    if (!isDead()) {
        restoreEnergy(defaultEnergyRecovery);
        clearExpiredStatuses();
        for (auto& status : pendingStatuses_) {
            addStatus(std::move(status));
        }
        pendingStatuses_.clear();
    }
}

bool Robot::attack(Robot& other) { return other.takeDamage(getAttack()); }

bool Robot::attack(Robot& other, int damage) {
    if (damage <= 0) {
        return false;
    }

    other.takeDamage(damage);
    return other.isDead();
}

bool Robot::takeDamage(int damage) {
    for (auto& status : getAllStatuses()) {
        damage = status->absorbDamage(damage);
    }

    if (damage > 0) {
        setHealth(health_ - damage);
        if (health_ == 0) {
            return true;
        }
    }
    return false;
}

void Robot::heal(int amount) {
    if (amount > 0) {
        setHealth(health_ + amount);
    }
}

InteractResult Robot::interact(Robot& other) {
    if (getTeam() != other.getTeam()) {
        bool killed = attack(other);
        if (killed) {
            return InteractResult::Kill;
        }
        return InteractResult::Damaged;
    }
    return InteractResult::NotInteracted;
}

int Robot::getHealCount() const { return healCount_; }

void Robot::setHealCount(int newHeal) {
    if (newHeal > 0) {
        healCount_ = newHeal;
    }
}

Team Robot::getTeam() const { return team_; }

void Robot::setTeam(Team team) { team_ = team; }

bool Robot::isDead() const { return health_ <= 0; }
