#pragma once

#include <array>

#include "Robot.hpp"
#include "game/items/Item.hpp"

constexpr int defaultExpiriense = 0;
constexpr int defaultLevel = 1;
constexpr int defaultPlayerSpeed = 2;
constexpr int defaultPlayerHeal = 4;
constexpr int defaultVisibiltyRadius = 5;

class Player : public Robot {
   private:
    int experience_ = defaultExpiriense;
    int level_ = defaultLevel;
    int visibiltyRadius_ = defaultVisibiltyRadius;
    std::array<int, 3> items_{};

   public:
    Player();
    ~Player();

    void setExperience(int experience);
    int getExperience() const;
    void setLevel(int level);
    int getLevel() const;
    int gainExp(int value) override;
    int expToNextLevel() const;
    void addItem(ItemType type);
    int getItemCount(ItemType type) const;
    bool useItem(ItemType type);

    void setVisibiltyRadius(int radius);
    int getVisibiltyRadius() const;
    bool isInRadius(const Point& target) const;

    InteractResult interact(Robot& other) override;

    void onTurnEnd();
};
