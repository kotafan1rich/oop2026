#include "game/Player.hpp"

#include <algorithm>
#include <cmath>
#include <string>

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
    int x1 = getPosition().getX();
    int x2 = target.getX();
    int y1 = getPosition().getY();
    int y2 = target.getY();

    return hypot(x2 - x1, y2 - y1) <= getVisibiltyRadius();
}

void Player::gainExp(int value) {
    if (value <= 0) {
        return;
    }

    experience_ += value;

    while (experience_ >= expToNextLevel()) {
        experience_ -= expToNextLevel();
        level_++;

        setMaxHealth(getMaxHealth() + 10);
        setAttack(getAttack() + 5);
        setMaxEnergy(getMaxEnergy() + 5);
    }
}

int Player::expToNextLevel() const { return getLevel() * 100; }

bool Player::interact(Robot& other) {
    if (other.getTeam() == Team::Friend) {
        heal(other.getHealCount());
        return false;
    }

    bool interacted = Robot::interact(other);
    if (other.isDead()) {
        gainExp(30);
    }
    return interacted;
}

void Player::onTurnEnd() {
    if (!isDead()) {
        restoreEnergy(defaultEnergyRecovery);
    }
}
