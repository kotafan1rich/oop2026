#pragma once

#include "Ability.hpp"

constexpr int rangedAttackCostDeafault = 1;
constexpr int rangedAttackRadiusDefault = 3;

class RangedAttach final : public Ability {
   private:
    int cost_ = rangedAttackCostDeafault;
    int range_ = rangedAttackRadiusDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target);
    int energyCost() const;
    int range() const;
    AbilityType getType() const;
};
