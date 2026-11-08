#include "terminal/Renderer.hpp"

#include <string>

Renderer::Renderer() {}

Renderer::~Renderer() {}

const RenderIcons& Renderer::getIcons() const { return icons_; }

std::string Renderer::renderWorld(const World& world) {
    const int width = world.getWidth();
    const int height = world.getHeight();

    const int rowLength = width;

    std::string buffer = "";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            buffer += cells_[y * rowLength + x].withColor();
        }
        buffer += "\n";
    }

    std::string interafce = getInterface(world);
    buffer += "\n";
    buffer += interafce;

    return buffer;
}

void Renderer::drawWorld(const World& world) {
    const int rowLength = world.getWidth();
    drawField(world, rowLength);
    drawFactory(world, rowLength);
    drawRobots(world, rowLength);
    drawPlayer(world, rowLength);
}

void Renderer::highlightCircle(const World& world, Point center, int radius) {
    const int width = world.getWidth();
    const int height = world.getHeight();

    auto insideRadius = [&](int x, int y) {
        int dx = x - center.getX();
        int dy = y - center.getY();
        return dx * dx + dy * dy <= radius * radius;
    };

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            bool isEdge = insideRadius(x, y) &&
                          (!insideRadius(x - 1, y) || !insideRadius(x + 1, y) ||
                           !insideRadius(x, y - 1) || !insideRadius(x, y + 1));

            if (isEdge) {
                cells_[y * width + x].background = Color::Red;
            }
        }
    }
}

void Renderer::highlightPoint(const World& world, Point point) {
    if (world.isInside(point)) {
        cells_[point.getY() * (world.getWidth()) + point.getX()].background =
            Color::Blue;
    }
}

void Renderer::drawField(const World& world, const int rowLength) {
    const int width = world.getWidth();
    const int height = world.getHeight();
    cells_.resize(height * rowLength);

    const Player& player = world.getPlayer();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Point point(x, y);
            const Cell& cell = world.getCell(point);
            RenderCell renderCell;
            renderCell.symbol =
                getTerrainSymbol(cell, player.isInRadius(point));
            setCell(point, rowLength, renderCell);
        }
    }
}

void Renderer::drawFactory(const World& world, const int rowLength) {
    for (const auto& factory : world.getFactories()) {
        if (factory == nullptr) {
            continue;
        }

        const Point& factoryPoint = factory->getPosition();
        for (int dy = 0; dy < factory->getHeight(); ++dy) {
            for (int dx = 0; dx < factory->getWidth(); ++dx) {
                Point point(factoryPoint.getX() + dx, factoryPoint.getY() + dy);
                const Cell& cell = world.getCell(point);

                if (cell.isSeen()) {
                    RenderCell renderCell;
                    renderCell.symbol = getFactorySymbol();
                    setCell(point, rowLength, renderCell);
                }
            }
        }
    }
}

void Renderer::drawRobots(const World& world, const int rowLength) {
    const Player& player = world.getPlayer();
    for (const auto& robot : world.getRobots()) {
        if (robot == nullptr) {
            continue;
        }
        if (player.isInRadius(robot->getPosition())) {
            RenderCell renderCell;
            renderCell.symbol = getRobotSymbol(*robot);
            renderCell.foreground = robot->getTeam() != player.getTeam()
                                        ? Color::Red
                                        : Color::Green;
            setCell(robot->getPosition(), rowLength, renderCell);
        }
    }
}

void Renderer::drawPlayer(const World& world, const int rowLength) {
    RenderIcons icons = getIcons();
    const Player& player = world.getPlayer();

    RenderCell renderCell;
    renderCell.symbol = icons.player;
    renderCell.foreground = Color::Green;

    if (player.isDead()) {
        renderCell.symbol = icons.deadRobot;
    }
    setCell(player.getPosition(), rowLength, renderCell);
}

std::string Renderer::getInterface(const World& world) {
    const Player& player = world.getPlayer();

    std::string buffer = "\nHealth: " + std::to_string(player.getHealth()) +
                         "/" + std::to_string(player.getMaxHealth());

    buffer += "  Energy: " + std::to_string(player.getEnergy()) + "/" +
              std::to_string(player.getMaxEnergy()) + "\n";

    buffer += "LEVEL " + std::to_string(player.getLevel()) + " - " +
              std::to_string(player.getExperience()) + "/" +
              std::to_string(player.expToNextLevel()) + '\n';

    buffer += "WASD: move  R/Space: rest  Q: quit  E: use ability\n";

    return buffer;
}

void Renderer::setCell(const Point& point, const int rowLength,
                       RenderCell cell) {
    int index = point.getY() * rowLength + point.getX();
    if (index >= 0 && index < cells_.size()) {
        cells_[index] = cell;
    }
}

char Renderer::getTerrainSymbol(const Cell& cell, bool inRadius) const {
    RenderIcons icons = getIcons();

    if (!cell.isSeen()) {
        return icons.floor;
    }

    TerrainType terrainType = cell.getTerrainType();
    if (terrainType == TerrainType::Floor) {
        if (inRadius) {
            return icons.floor;
        } else {
            return icons.unknown;
        }
    }

    switch (terrainType) {
        case TerrainType::Wall:
            return icons.wall;
        case TerrainType::Swamp:
            return icons.swamp;
        default:
            return icons.unknown;
    }
}

char Renderer::getRobotSymbol(const Robot& robot) const {
    RenderIcons icons = getIcons();

    if (robot.isDead()) {
        return icons.deadRobot;
    }
    if (robot.getTeam() == Team::Friend) {
        return icons.friendly;
    }

    return icons.enemy;
}

char Renderer::getFactorySymbol() const {
    RenderIcons icons = getIcons();
    return icons.factory;
}

std::string RenderCell::withColor() const {
    std::string result;

    switch (foreground) {
        case Color::Red:
            result += "\033[31m";
            break;
        case Color::Green:
            result += "\033[32m";
            break;
        case Color::Yellow:
            result += "\033[33m";
            break;
        case Color::Blue:
            result += "\033[34m";
            break;
        case Color::Default:
            break;
    }

    switch (background) {
        case Color::Red:
            result += "\033[41m";
            break;
        case Color::Green:
            result += "\033[42m";
            break;
        case Color::Yellow:
            result += "\033[43m";
            break;
        case Color::Blue:
            result += "\033[44m";
            break;
        case Color::Default:
            break;
    }

    result += symbol;

    if (foreground != Color::Default || background != Color::Default) {
        result += "\033[0m";
    }

    return result;
}
