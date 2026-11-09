#pragma once

#include "Status.hpp"

class Slowdown final : public Status {
   private:
    int remainingTurns_;

   public:
    explicit Slowdown(int turns = 1);
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    bool blocksMovement() const override;
    void onTurnEnd(Robot& owner) override;
};
