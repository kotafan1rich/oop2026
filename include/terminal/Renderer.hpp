#pragma once

#include "game/world/World.hpp"

struct RenderIcons {
    char floor = ' ';
    char wall = '#';
    char swamp = '~';
    char player = '&';
    char enemy = '@';
    char friendly = 'H';
    char factory = '+';
    char deadRobot = '%';
    char unknown = '*';
};

enum class Color {
    Default,
    Red,
    Green,
    Yellow,
    Blue,
};

struct RenderCell {
    char symbol = ' ';
    Color foreground = Color::Default;
    Color background = Color::Default;

    std::string withColor() const;
};

class Renderer {
   private:
    RenderIcons icons_;
    std::vector<RenderCell> cells_;

   public:
    Renderer();
    ~Renderer();

    const RenderIcons& getIcons() const;

    std::string renderWorld(const World& world);

    void highlightCircle(const World& world, Point center, int radius);
    void highlightPoint(const World& world, Point point);

    void drawWorld(const World& world);
    void drawField(const World& world, const int rowLength);
    void drawFactory(const World& world, const int rowLength);
    void drawRobots(const World& world, const int rowLength);
    void drawPlayer(const World& world, const int rowLength);

    std::string getInterface(const World& world);

    void setCell(const Point& point, const int rowLength, RenderCell cell);

    char getTerrainSymbol(const Cell& cell, bool inRadius) const;
    char getRobotSymbol(const Robot& robot) const;
    char getFactorySymbol() const;
};
