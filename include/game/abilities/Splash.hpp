#pragma once

#include <memory>

#include "Ability.hpp"
#include "game/statuses/Status.hpp"

constexpr int splashCostDefault = 10;
constexpr int splashRangeDefault = 1;
constexpr StatusConfig splashOverloadDefault{StatusType::Overload, 0, 0};

class Splash final : public Ability {
   private:
    int cost_ = splashCostDefault;
    int range_ = splashRangeDefault;
    StatusConfig appliedStatusConfig_ = splashOverloadDefault;

   public:
    bool use(Robot& owner, World& world, std::optional<Point> target) override;
    int energyCost() const override;
    int range() const override;
    AbilityType getType() const override;
    std::unique_ptr<Status> createAppliedStatus();
    void upgrade() override;
};
