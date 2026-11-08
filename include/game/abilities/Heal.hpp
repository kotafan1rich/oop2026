#pragma once

#include "Ability.hpp"
#include "game/units/Robot.hpp"

constexpr int healCostDeafault = 1;
constexpr int healCountDeafault = 20;

class Heal final : public Ability {
   private:
    int cost_ = healCostDeafault;
    int healCount_ = healCountDeafault;
    int range_ = 0;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target);
    int energyCost() const;
    int range() const;
    AbilityType getType() const;
    void upgrade();
};