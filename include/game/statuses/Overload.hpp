#pragma once

#include "Status.hpp"

class Overload final : public Status {
   private:
    bool consumed_ = false;

   public:
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    bool blockAbilityAttempt() override;
};
