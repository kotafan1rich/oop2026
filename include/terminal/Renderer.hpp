#pragma once

#include <string_view>

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

struct RenderStatuses {
    static constexpr std::string_view slowdown = "\033[30;43m Slowdown \033[0m";
    static constexpr std::string_view overload = "\033[30;45m Overload \033[0m";
    static constexpr std::string_view burning = "\033[37;41m Burning \033[0m";
    static constexpr std::string_view shield = "\033[37;44m Shield \033[0m";
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
    std::string getStatusStat(const Player& player);

    void setCell(const Point& point, const int rowLength, RenderCell cell);

    char getTerrainSymbol(const Cell& cell, bool inRadius) const;
    char getRobotSymbol(const Robot& robot) const;
    char getFactorySymbol() const;
    std::string getStatusText(StatusType statusType);
};
