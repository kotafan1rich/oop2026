#pragma once

#include "Status.hpp"

class Shield final : public Status {
   private:
    int remainingShield_;

   public:
    explicit Shield(int amount);
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    int absorbDamage(int damage) override;
};
