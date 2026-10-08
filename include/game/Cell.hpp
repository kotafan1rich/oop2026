#pragma once

#include "Factory.hpp"
#include "Point.hpp"
#include "Robot.hpp"

constexpr int defaultMovementCost = 1;
constexpr int swampMovementCost = 2;
constexpr int impassableMovementCost = 0;

enum class TerrainType { Floor, Wall, Swamp };

class Cell {
   private:
    TerrainType terrainType_ = TerrainType::Floor;
    int movementCost_;
    bool isSeen_ = false;
    Robot* robot_ = nullptr;
    Factory* factory_ = nullptr;

   public:
    Cell(int movementCost = defaultMovementCost);
    ~Cell();

    TerrainType getTerrainType() const;
    void setTerrainType(TerrainType type);

    bool isWalkable() const;

    int getMovementCost() const;
    void setMovementCost(int movementCost);

    void setRobot(Robot* robot);
    Robot* getRobot() const;
    Robot* removeRobot();

    void setFactory(Factory* factory);
    Factory* getFactory() const;
    Factory* removeFactory();

    void markSeen();
    bool isSeen() const;

    bool isTaken() const;
};
