#pragma once

#include "Ability.hpp"

constexpr int rangedAttackCostDeafault = 5;
constexpr int rangedAttackRadiusDefault = 3;
constexpr int rangedDamageDefault = 15;

class RangedAttach final : public Ability {
   private:
    int cost_ = rangedAttackCostDeafault;
    int range_ = rangedAttackRadiusDefault;
    int damage_ = rangedDamageDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target);
    int energyCost() const;
    int range() const;
    AbilityType getType() const;
    void upgrade();
};
