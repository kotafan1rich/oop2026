#pragma once

#include <memory>
#include <vector>

#include "game/abilities/Ability.hpp"
#include "game/statuses/Status.hpp"
#include "game/world/Point.hpp"

constexpr int maxHealth = 100;
constexpr int defaultAttack = 10;
constexpr int defaultSpeed = 1;
constexpr int defaultEnergy = 100;
constexpr int defaultMaxEnergy = 100;
constexpr int defaultEnergyRecovery = 1;
constexpr int defaultRobotHeal = 10;

enum class Team { Friend, Enemy };

enum class InteractResult {
    NotInteracted,
    HealSelf,
    Kill,
    Damaged,
};

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
    std::vector<std::unique_ptr<Status>> statuses_;
    std::vector<std::unique_ptr<Status>> pendingStatuses_;

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
    int getBaseAttack() const;

    void setSpeed(int speed);
    int getSpeed() const;

    void setEnergy(int energy);
    int getEnergy() const;
    void setMaxEnergy(int maxEnergy);
    int getMaxEnergy() const;
    void expendEnergy(int value);
    void restoreEnergy(int value);

    virtual int gainExp(int value);

    void addAbility(std::unique_ptr<Ability> ability);
    const std::vector<std::unique_ptr<Ability>>& getAllAbilities() const;
    std::vector<AbilityType> getAllAbilityTypes() const;
    std::optional<std::reference_wrapper<Ability>> getAbility(
        AbilityType abilityType) const;
    bool upgradeAbility(AbilityType abilityType);

    void addStatus(std::unique_ptr<Status> status);
    void addPendingStatus(std::unique_ptr<Status> status);
    const std::vector<std::unique_ptr<Status>>& getAllStatuses() const;
    std::optional<std::reference_wrapper<Status>> getStatus(
        StatusType statusType) const;
    void clearExpiredStatuses();

    bool attack(Robot& other);
    bool attack(Robot& other, int damage);
    bool takeDamage(int amount);
    void heal(int amount);
    virtual InteractResult interact(Robot& other);

    int getHealCount() const;
    void setHealCount(int newHeal);

    Team getTeam() const;
    void setTeam(Team team);

    bool isDead() const;

    virtual void move(int dx, int dy);

    virtual void onTurnEnd();
};
