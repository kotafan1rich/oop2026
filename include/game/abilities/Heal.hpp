#pragma once

#include <memory>

#include "Ability.hpp"
#include "game/units/Robot.hpp"

constexpr int healCostDeafault = 5;
constexpr int healCountDeafault = 20;

class Heal final : public Ability {
   private:
    int cost_ = healCostDeafault;
    int healCount_ = healCountDeafault;
    int range_ = 0;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target) override;
    int energyCost() const override;
    int range() const override;
    AbilityType getType() const override;
    std::unique_ptr<Status> createAppliedStatus(int value);
    void upgrade() override;
};