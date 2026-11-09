#pragma once

#include <memory>

#include "Ability.hpp"
#include "game/statuses/Status.hpp"

constexpr int rangedAttackCostDeafault = 5;
constexpr int rangedAttackRadiusDefault = 3;
constexpr int rangedDamageDefault = 15;
constexpr StatusConfig rangedBurningDefault{StatusType::Burning, 3, 5};

class RangedAttach final : public Ability {
   private:
    int cost_ = rangedAttackCostDeafault;
    int range_ = rangedAttackRadiusDefault;
    int damage_ = rangedDamageDefault;
    StatusConfig appliedStatusConfig_ = rangedBurningDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target) override;
    int energyCost() const override;
    int range() const override;
    AbilityType getType() const override;
    void addAppliedStatus(StatusConfig statusConfig);
    std::unique_ptr<Status> createAppliedStatus();
    void upgrade() override;
};
