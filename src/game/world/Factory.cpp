#include "game/world/Factory.hpp"

#include "game/abilities/Heal.hpp"
#include "game/abilities/RangedAttach.hpp"
#include "game/abilities/Splash.hpp"
#include "game/abilities/Teleport.hpp"
#include "game/units/Robot.hpp"

Factory::Factory(RobotConfig robotConfig, int height, int width)
    : robotConfig_(robotConfig), height_(height), width_(width) {
    generator = std::mt19937(std::random_device{}());
}

Factory::~Factory() {}

void Factory::setX(int x) { point_.setX(x); }

void Factory::setY(int y) { point_.setY(y); }

int Factory::getX() const { return point_.getX(); }

int Factory::getY() const { return point_.getY(); }

Point& Factory::getPosition() { return point_; }

const Point& Factory::getPosition() const { return point_; }

void Factory::setPosition(const Point& point) {
    setX(point.getX());
    setY(point.getY());
}

void Factory::setSpawnInterval(int interval) {
    if (interval > 0) {
        spawnInterval_ = interval;
    }
}

int Factory::getSpawnInterval() const { return spawnInterval_; }

int Factory::getHeight() const { return height_; }

int Factory::getWidth() const { return width_; }

AbilityType Factory::getRandomAbility() {
    std::uniform_int_distribution<int> choice(0, 1);
    AbilityType abilityType =
        choice(generator) == 0 ? AbilityType::Heal : AbilityType::Teleport;
    return abilityType;
}

std::unique_ptr<Robot> Factory::spawn() {
    if (stepCounter_ != getSpawnInterval() - 1) {
        return nullptr;
    }

    auto newRobot = std::make_unique<Robot>(robotConfig_.speed);
    newRobot->setAttack(robotConfig_.attack);
    newRobot->setMaxEnergy(robotConfig_.maxEnergy);
    newRobot->setMaxHealth(robotConfig_.maxHealth);

    AbilityType abilityType = getRandomAbility();

    switch (abilityType) {
        case AbilityType::Heal:
            newRobot->addAbility(std::make_unique<Heal>());
            break;

        case AbilityType::RangedAttack:
            newRobot->addAbility(std::make_unique<RangedAttach>());
            break;
        case AbilityType::Splash:
            newRobot->addAbility(std::make_unique<Splash>());
            break;
        case AbilityType::Teleport:
            newRobot->addAbility(std::make_unique<Teleport>());
            break;
    }

    return newRobot;
}

void Factory::onTurnEnd() {
    stepCounter_ = (stepCounter_ + 1) % (getSpawnInterval());
}