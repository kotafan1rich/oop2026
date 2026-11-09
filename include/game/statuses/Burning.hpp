#pragma once

#include "Status.hpp"

class Burning final : public Status {
   private:
    int remainingTurns_;
    int damage_;

   public:
    explicit Burning(int turns, int damage = 5);
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    void onTurnEnd(Robot& owner) override;
};
