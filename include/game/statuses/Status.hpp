#pragma once

class Robot;

enum class StatusType { Slowdown, Overload, Burning, Shield };

struct StatusConfig {
    StatusType stausType;
    int turns;
    int value;
};

class Status {
   public:
    virtual ~Status() = default;

    virtual StatusType getType() const = 0;
    virtual bool isExpire() const = 0;
    virtual void repeat(const Status& status) = 0;

    virtual bool blocksMovement() const { return false; }
    virtual bool blockAbilityAttempt() { return false; }
    virtual int absorbDamage(int damage) { return damage; }
    virtual void onTurnEnd(Robot&) {}
};
