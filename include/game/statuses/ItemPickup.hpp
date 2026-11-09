#pragma once

#include "Status.hpp"
#include "game/items/Item.hpp"

class Player;

class ItemPickup final : public Status {
   private:
    ItemType item_;
    bool collected_ = false;

   public:
    explicit ItemPickup(ItemType item);
    StatusType getType() const override;
    bool isExpire() const override;
    void repeat(const Status& status) override;
    void apply(Player& player);
};
