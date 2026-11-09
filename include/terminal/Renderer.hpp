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
    static constexpr std::string_view attackBoost = "\033[30;42m Attack Boost \033[0m";
};

enum class Color {
    Default,
    Red,
    Green,
    Yellow,
    Blue,
};

enum class BacklightMode {
    Default,
    Enemies,
    WalkableCells,
    StatusCells,

    Count_,
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

    std::string renderWorld(const World& world, BacklightMode mode);

    void highlightCircle(const World& world, Point center, int radius);
    void highlightPoint(const World& world, Point point);

    void drawWorld(const World& world, BacklightMode mode);
    void drawField(const World& world, BacklightMode mode);
    void drawFactory(const World& world);
    void drawRobots(const World& world, BacklightMode mode);
    void drawPlayer(const World& world, BacklightMode mode);

    RenderCell getTerrainRenderCell(BacklightMode mode, const Cell& cell,
                                    bool inRadius);

    std::string getInterface(const World& world, BacklightMode mode);
    std::string getStatusStat(const Player& player);
    std::string getBacklightModes(BacklightMode mode);

    void setCell(const Point& point, const int rowLength, RenderCell cell);

    char getTerrainSymbol(const Cell& cell, bool inRadius) const;
    char getRobotSymbol(const Robot& robot) const;
    char getFactorySymbol() const;

    Color getTerrainColor(BacklightMode mode, const Cell& cell,
                          bool inRadius) const;
    Color getRobotColor(BacklightMode mode, const Player& player,
                        const Robot& robot) const;

    std::string getStatusText(StatusType statusType);
    std::string getModeText(BacklightMode currentMode,
                            BacklightMode mode) const;
};
