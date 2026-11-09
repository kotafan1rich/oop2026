#include "game/statuses/ItemPickup.hpp"

#include "game/units/Player.hpp"

ItemPickup::ItemPickup(ItemType item) : item_(item) {}
StatusType ItemPickup::getType() const { return StatusType::ItemPickup; }
bool ItemPickup::isExpire() const { return collected_; }
void ItemPickup::repeat(const Status&) {}

void ItemPickup::apply(Player& player) {
    if (!collected_) {
        player.addItem(item_);
        collected_ = true;
    }
}
