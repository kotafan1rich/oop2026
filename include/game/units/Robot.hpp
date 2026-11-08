#pragma once

#include <memory>
#include <vector>

#include "game/abilities/Ability.hpp"
#include "game/world/Point.hpp"

constexpr int maxHealth = 100;
constexpr int defaultAttack = 10;
constexpr int defaultSpeed = 1;
constexpr int defaultEnergy = 100;
constexpr int defaultMaxEnergy = 100;
constexpr int defaultEnergyRecovery = 1;
constexpr int defaultRobotHeal = 10;

enum class Team { Friend, Enemy };

class Robot {
   private:
    Point point_ = Point(0, 0);
    Team team_ = Team::Enemy;
    int health_ = maxHealth;
    int maxHealth_ = maxHealth;
    int attack_ = defaultAttack;
    int speed_ = defaultSpeed;
    int energy_ = defaultEnergy;
    int maxEnergy_ = defaultMaxEnergy;
    int healCount_ = defaultRobotHeal;
    std::vector<std::unique_ptr<Ability>> abilities_;

   public:
    explicit Robot(int speed = defaultSpeed);
    virtual ~Robot();

    void setX(int x);
    void setY(int y);
    int getX() const;
    int getY() const;
    Point& getPosition();
    const Point& getPosition() const;
    void setPosition(const Point& point);

    void setHealth(int health);
    int getHealth() const;
    void setMaxHealth(int maxHealth);
    int getMaxHealth() const;

    void setAttack(int attack);
    int getAttack() const;

    void setSpeed(int speed);
    int getSpeed() const;

    void setEnergy(int energy);
    int getEnergy() const;
    void setMaxEnergy(int maxEnergy);
    int getMaxEnergy() const;
    void expendEnergy(int value);
    void restoreEnergy(int value);

    virtual bool gainExp(int value);

    void addAbility(std::unique_ptr<Ability> ability);
    const std::vector<std::unique_ptr<Ability>>& getAllAbilities() const;
    std::vector<AbilityType> getAllAbilityTypes() const;
    std::optional<std::reference_wrapper<Ability>> getAbility(
        AbilityType abilityType) const;

    bool attack(Robot& other);
    bool takeDamage(int amount);
    void heal(int amount);
    virtual bool interact(Robot& other);

    int getHealCount() const;
    void setHealCount(int newHeal);

    Team getTeam() const;
    void setTeam(Team team);

    bool isDead() const;

    virtual void move(int dx, int dy);

    virtual void onTurnEnd();
};
