#pragma once

#include "Factory.hpp"
#include "Point.hpp"
#include "Robot.hpp"

constexpr int defaultMovementCost = 1;
constexpr int swampMovementCost = 2;
constexpr int impassableMovementCost = 0;

enum class TerrainType { Floor, Wall, Swamp };

enum class OccupantType {
    Empty,
    Factory,
    Robot,
};

class Cell {
   private:
    TerrainType terrainType_ = TerrainType::Floor;
    int movementCost_;
    bool isSeen_ = false;
    OccupantType occupant_ = OccupantType::Empty;

   public:
    Cell(int movementCost = defaultMovementCost);
    ~Cell();

    TerrainType getTerrainType() const;
    void setTerrainType(TerrainType type);

    bool isWalkable() const;

    int getMovementCost() const;
    void setMovementCost(int movementCost);

    OccupantType getOccupantType() const;
    void setOccupantType(OccupantType occupant);
    void clearOccupant();

    void markSeen();
    bool isSeen() const;

    bool isTaken() const;
};
