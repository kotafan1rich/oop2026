#pragma once

#include <optional>

#include "game/world/Point.hpp"

class Robot;
class World;

enum class AbilityType { Splash, RangedAttack, Heal, Teleport };

class Ability {
   public:
    virtual ~Ability() = default;

    virtual bool use(Robot& owner, World& world,
                     std::optional<Point> target) = 0;
    virtual int energyCost() const = 0;
    virtual int range() const = 0;
    virtual AbilityType getType() const = 0;
};
