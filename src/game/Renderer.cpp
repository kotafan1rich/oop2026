#include "game/Renderer.hpp"

#include <string>
#include <vector>

Renderer::Renderer() {}

Renderer::~Renderer() {}

const RenderIcons& Renderer::getIcons() const { return icons_; }

std::string_view Renderer::render(const World& world) {
    const int rowLength = world.getWidth() + 1;
    drawField(world, rowLength);
    drawFactory(world, rowLength);
    drawRobots(world, rowLength);
    drawPlayer(world, rowLength);
    drawInterface(world);

    return buffer_;
}

void Renderer::drawField(const World& world, const int rowLength) {
    const int width = world.getWidth();
    const int height = world.getHeight();
    buffer_.resize(height * rowLength);

    const Player& player = world.getPlayer();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Point point(x, y);
            const Cell& cell = world.getCell(point);
            setSymbol(point, rowLength,
                      getTerrainSymbol(cell, player.isInRadius(point)));
        }
        setSymbol(Point(width, y), rowLength, '\n');
    }
}

void Renderer::drawFactory(const World& world, const int rowLength) {
    RenderIcons icons = getIcons();
    const Player& player = world.getPlayer();

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
                    setSymbol(point, rowLength,
                              getFactorySymbol(player.isInRadius(point)));
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
            setSymbol(robot->getPosition(), rowLength, getRobotSymbol(*robot));
        }
    }
}

void Renderer::drawPlayer(const World& world, const int rowLength) {
    RenderIcons icons = getIcons();
    const Player& player = world.getPlayer();
    char symb = icons.player;
    if (player.isDead()) {
        symb = icons.deadRobot;
    }
    setSymbol(player.getPosition(), rowLength, symb);
}

void Renderer::drawInterface(const World& world) {
    const Player& player = world.getPlayer();

    buffer_ += "\nHealth: " + std::to_string(player.getHealth()) + "/" +
               std::to_string(player.getMaxHealth());

    buffer_ += "  Energy: " + std::to_string(player.getEnergy()) + "/" +
               std::to_string(player.getMaxEnergy()) + "\n";

    buffer_ += "LEVEL " + std::to_string(player.getLevel()) + " - " +
               std::to_string(player.getExperience()) + "/" +
               std::to_string(player.expToNextLevel()) + '\n';

    buffer_ += "WASD: move  R/Space: rest  Q: quit\n";
}

void Renderer::setSymbol(const Point& point, const int rowLength, char symbol) {
    int index = point.getY() * rowLength + point.getX();
    if (index >= 0 && index < buffer_.length()) {
        buffer_[index] = symbol;
    }
}

char Renderer::getTerrainSymbol(const Cell& cell, bool inRadius) const {
    RenderIcons icons = getIcons();

    if (!cell.isSeen()) {
        return icons.floor;
    }

    TerrainType terrainType = cell.getTerrainType();
    if (terrainType == TerrainType::Floor) {
        return icons.floor;
    }
    if (!inRadius) {
        return icons.unknown;
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

char Renderer::getFactorySymbol(bool inRadius) const {
    RenderIcons icons = getIcons();
    if (inRadius) {
        return icons.factory;
    } else {
        return icons.unknown;
    }
}
