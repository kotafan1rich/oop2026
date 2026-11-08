#pragma once

#include "Ability.hpp"

constexpr int teleportCostDefault = 10;
constexpr int teleportRangeDefault = 5;

class Teleport final : public Ability {
   private:
    int cost_ = teleportCostDefault;
    int range_ = teleportRangeDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target);
    int range() const;
    int energyCost() const;
    AbilityType getType() const;
    void upgrade();
};