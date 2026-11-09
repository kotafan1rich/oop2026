#pragma once

#include "Status.hpp"

class AttackBoost final : public Status {
   private:
    int remainingTurns_;
    int bonus_;

   public:
    AttackBoost(int turns, int bonus);
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    int attackBonus() const override;
    void onTurnEnd(Robot& owner) override;
};
