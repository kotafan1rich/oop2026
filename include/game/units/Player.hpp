#pragma once

#include "Robot.hpp"

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

   public:
    Player();
    ~Player();

    void setExperience(int experience);
    int getExperience() const;
    void setLevel(int level);
    int getLevel() const;
    bool gainExp(int value) override;
    int expToNextLevel() const;

    void setVisibiltyRadius(int radius);
    int getVisibiltyRadius() const;
    bool isInRadius(const Point& target) const;

    bool interact(Robot& other) override;

    void onTurnEnd();
};
