#include "game/units/Player.hpp"

Player::Player() : Robot(defaultPlayerSpeed) {
    setTeam(Team::Friend);
    setHealCount(defaultPlayerHeal);
}

Player::~Player() {}

void Player::setExperience(int experience) {
    if (experience >= 0) {
        experience_ = experience;
    }
}

int Player::getExperience() const { return experience_; }

void Player::setLevel(int level) {
    if (level > 0) {
        level_ = level;
    }
}

int Player::getLevel() const { return level_; }

void Player::setVisibiltyRadius(int radius) {
    if (radius > 0) {
        visibiltyRadius_ = radius;
    }
}

int Player::getVisibiltyRadius() const { return visibiltyRadius_; }

bool Player::isInRadius(const Point& target) const {
    return getPosition().dist(target) <= getVisibiltyRadius();
}

int Player::gainExp(int value) {
    if (value <= 0) {
        return 0;
    }

    experience_ += value;
    int levelBefore = level_;
    while (experience_ >= expToNextLevel()) {
        experience_ -= expToNextLevel();
        level_++;

        setMaxHealth(getMaxHealth() + 10);
        setAttack(getAttack() + 5);
        setMaxEnergy(getMaxEnergy() + 5);
    }
    return level_ - levelBefore;
}

int Player::expToNextLevel() const { return getLevel() * 100; }

InteractResult Player::interact(Robot& other) {
    if (other.getTeam() == Team::Friend) {
        heal(other.getHealCount());
        return InteractResult::HealSelf;
    }

    return Robot::interact(other);
}

void Player::onTurnEnd() {
    if (!isDead()) {
        restoreEnergy(defaultEnergyRecovery);
    }
}
