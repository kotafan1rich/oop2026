#include "terminal/Renderer.hpp"

#include <string>

Renderer::Renderer() {}

Renderer::~Renderer() {}

const RenderIcons& Renderer::getIcons() const { return icons_; }

std::string Renderer::renderWorld(const World& world, BacklightMode mode) {
    const int width = world.getWidth();
    const int height = world.getHeight();

    const int rowLength = width;

    std::string buffer = "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            buffer += cells_[y * rowLength + x].withColor();
        }
        buffer += "\n";
    }

    std::string interafce = getInterface(world, mode);
    buffer += "\n";
    buffer += interafce;

    return buffer;
}

void Renderer::drawWorld(const World& world, BacklightMode mode) {
    drawField(world, mode);
    drawFactory(world);
    drawRobots(world, mode);
    drawPlayer(world, mode);
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
            Color::Yellow;
    }
}

void Renderer::drawField(const World& world, BacklightMode mode) {
    const int width = world.getWidth();
    const int height = world.getHeight();
    cells_.resize(height * width);

    const Player& player = world.getPlayer();
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Point point(x, y);
            bool inRadius = player.isInRadius(point);
            const Cell& cell = world.getCell(point);
            RenderCell renderCell = getTerrainRenderCell(mode, cell, inRadius);
            setCell(point, width, renderCell);
        }
    }
}

void Renderer::drawFactory(const World& world) {
    const int rowLength = world.getWidth();
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
                    RenderCell renderCell =
                        cells_[point.getY() * rowLength + point.getX()];
                    renderCell.symbol = getFactorySymbol();
                    setCell(point, rowLength, renderCell);
                }
            }
        }
    }
}

void Renderer::drawRobots(const World& world, BacklightMode mode) {
    const int rowLength = world.getWidth();
    const Player& player = world.getPlayer();
    for (const auto& robot : world.getRobots()) {
        if (robot == nullptr) {
            continue;
        }
        if (player.isInRadius(robot->getPosition())) {
            const Cell& cell = world.getCell(robot->getPosition());
            RenderCell renderCell;
            renderCell.symbol = getRobotSymbol(*robot);
            renderCell.foreground = getRobotColor(mode, player, *robot);
            renderCell.background = getTerrainColor(mode, cell, true);
            setCell(robot->getPosition(), rowLength, renderCell);
        }
    }
}

void Renderer::drawPlayer(const World& world, BacklightMode mode) {
    RenderIcons icons = getIcons();
    const Player& player = world.getPlayer();
    const Cell& cell = world.getCell(player.getPosition());

    RenderCell renderCell;
    renderCell.symbol = icons.player;
    renderCell.foreground = Color::Green;
    renderCell.background = getTerrainColor(mode, cell, true);

    if (player.isDead()) {
        renderCell.symbol = icons.deadRobot;
    }
    setCell(player.getPosition(), world.getWidth(), renderCell);
}

RenderCell Renderer::getTerrainRenderCell(BacklightMode mode, const Cell& cell,
                                          bool inRadius) {
    RenderCell renderCell;
    renderCell.symbol = getTerrainSymbol(cell, inRadius);
    renderCell.background = getTerrainColor(mode, cell, inRadius);
    return renderCell;
}

std::string Renderer::getInterface(const World& world, BacklightMode mode) {
    const Player& player = world.getPlayer();

    std::string buffer = getStatusStat(player) + "\n";

    buffer += "\n" + getBacklightModes(mode) + "\n";

    buffer += "\nHealth: " + std::to_string(player.getHealth()) + "/" +
              std::to_string(player.getMaxHealth());

    buffer += "  Energy: " + std::to_string(player.getEnergy()) + "/" +
              std::to_string(player.getMaxEnergy()) + "\n";

    buffer += "LEVEL " + std::to_string(player.getLevel()) + " - " +
              std::to_string(player.getExperience()) + "/" +
              std::to_string(player.expToNextLevel()) + '\n';

    buffer += "Items: 1 Health " + std::to_string(player.getItemCount(ItemType::Health)) +
              "  2 Energy " + std::to_string(player.getItemCount(ItemType::Energy)) +
              "  3 Attack " + std::to_string(player.getItemCount(ItemType::Attack)) + "\n";
    buffer += "WASD: move  R/Space: rest  Q: quit  E: use ability  Tab: mode\n";

    return buffer;
}

std::string Renderer::getStatusStat(const Player& player) {
    std::string buffer = "";
    for (auto& status : player.getAllStatuses()) {
        if (status->isExpire()) {
            continue;
        }
        buffer += " " + getStatusText(status->getType());
    }

    return buffer;
}

std::string Renderer::getBacklightModes(BacklightMode mode) {
    const int count = static_cast<int>(BacklightMode::Count_);
    std::string buffer = "";
    for (int i = 0; i < count; i++) {
        BacklightMode currentMode = static_cast<BacklightMode>(i);
        buffer += " " + getModeText(mode, currentMode);
    }

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

Color Renderer::getTerrainColor(BacklightMode mode, const Cell& cell,
                                bool inRadius) const {
    switch (mode) {
        case BacklightMode::WalkableCells:
            if (!cell.isWalkable() && cell.isSeen()) {
                return Color::Yellow;
            }
            break;
        case BacklightMode::StatusCells:
            if (cell.hasStatus() && inRadius) {
                return Color::Blue;
            }
    };
    return Color::Default;
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

Color Renderer::getRobotColor(BacklightMode mode, const Player& player,
                              const Robot& robot) const {
    switch (mode) {
        case BacklightMode::Enemies:
            if (player.getTeam() != robot.getTeam()) {
                return Color::Red;
            } else {
                return Color::Green;
            }
    };
    return Color::Default;
}

char Renderer::getFactorySymbol() const {
    RenderIcons icons = getIcons();
    return icons.factory;
}

std::string Renderer::getStatusText(StatusType statusType) {
    switch (statusType) {
        case StatusType::Slowdown:
            return std::string(RenderStatuses::slowdown);
        case StatusType::Overload:
            return std::string(RenderStatuses::overload);
        case StatusType::Burning:
            return std::string(RenderStatuses::burning);
        case StatusType::Shield:
            return std::string(RenderStatuses::shield);
        case StatusType::AttackBoost:
            return std::string(RenderStatuses::attackBoost);
        case StatusType::ItemPickup:
            return {};
    }
    return {};
}

std::string Renderer::getModeText(BacklightMode selectedMode,
                                  BacklightMode mode) const {
    std::string text;
    switch (mode) {
        case BacklightMode::Default:
            text += "Обычный";
            break;
        case ::BacklightMode::Enemies:
            text += "Враги";
            break;
        case ::BacklightMode::StatusCells:
            text += "Статусы";
            break;
        case ::BacklightMode::WalkableCells:
            text += "Непроходимые";
            break;

        default:
            break;
    }
    if (mode == selectedMode) {
        text = "\033[44m" + text + "\033[0m";
    }

    return text;
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
