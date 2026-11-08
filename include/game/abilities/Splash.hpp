#pragma once

#include "Ability.hpp"

constexpr int splashCostDefault = 10;
constexpr int splashRangeDefault = 1;

class Splash final : public Ability {
   private:
    int cost_ = splashCostDefault;
    int range_ = splashRangeDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target);
    int energyCost() const;
    int range() const;
    AbilityType getType() const;
};