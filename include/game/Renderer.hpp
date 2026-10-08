#pragma once

#include <string>

#include "game/World.hpp"

struct RenderIcons {
    char floor = ' ';
    char wall = '#';
    char swamp = '~';
    char player = '&';
    char enemy = '@';
    char friendly = 'H';
    char factory = '+';
    char deadRobot = '%';
    char unknown = '?';
};

class Renderer {
   private:
    RenderIcons icons_;
    std::string buffer_;

   public:
    Renderer();
    ~Renderer();

    const RenderIcons& getIcons() const;

    std::string_view render(const World& world);
    void drawField(const World& world, const int rowLength);
    void drawFactory(const World& world, const int rowLength);
    void drawRobots(const World& world, const int rowLength);
    void drawPlayer(const World& world, const int rowLength);
    void drawInterface(const World& world);
    void setSymbol(const Point& point, const int rowLength, char symbol);

    char getTerrainSymbol(const Cell& cell) const;
    char getRobotSymbol(const Robot& robot) const;
};
