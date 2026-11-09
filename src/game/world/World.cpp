#include "game/world/World.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <exception>
#include <queue>

#include "game/navigation/PathFinder.hpp"
#include "game/statuses/Burning.hpp"
#include "game/statuses/ItemPickup.hpp"
#include "game/statuses/Overload.hpp"
#include "game/statuses/Shield.hpp"
#include "game/statuses/Slowdown.hpp"
#include "game/units/Robot.hpp"
#include "game/world/Cell.hpp"

void World::initDefaultLevel() {
    if (width_ >= 12 && height_ >= 12) {
        const int leftWall = width_ / 3;
        const int rightWall = width_ * 2 / 3;
        const int middleRow = height_ / 2;
        const int topPassage = height_ / 4;
        const int bottomPassage = height_ * 3 / 4;
        const int leftPassage = width_ / 4;
        const int rightPassage = width_ * 3 / 4;

        auto makeObstacle = [this](int x, int y) {
            Cell& cell = field_[y][x];
            cell.setMovementCost(impassableMovementCost);
            cell.setTerrainType(TerrainType::Wall);
        };

        for (int y = 2; y < height_ - 2; ++y) {
            if (y != topPassage && y != topPassage + 1 && y != middleRow &&
                y != bottomPassage) {
                makeObstacle(leftWall, y);
            }

            if (y != topPassage && y != middleRow && y != bottomPassage - 1 &&
                y != bottomPassage) {
                makeObstacle(rightWall, y);
            }
        }

        for (int x = 2; x < width_ - 2; ++x) {
            if (x != leftPassage && x != leftPassage + 1 && x != leftWall &&
                x != width_ / 2 && x != rightWall && x != rightPassage &&
                x != rightPassage + 1) {
                makeObstacle(x, middleRow);
            }
        }

        const int upperRow = height_ / 4;
        for (int x = leftWall + 3; x < rightWall - 2; ++x) {
            if (x != width_ / 2) {
                makeObstacle(x, upperRow);
            }
        }

        const int lowerRow = height_ * 3 / 4;
        for (int x = leftWall + 2; x < rightWall - 2; ++x) {
            if (x != width_ / 2 && x != width_ / 2 + 1) {
                makeObstacle(x, lowerRow);
            }
        }

        auto makeSwamp = [this](int x, int y) {
            Cell& cell = field_[y][x];
            if (cell.isWalkable()) {
                cell.setMovementCost(swampMovementCost);
                cell.setTerrainType(TerrainType::Swamp);
            }
        };

        for (int y = topPassage + 2; y < middleRow - 1; ++y) {
            for (int x = 2; x < leftWall - 1; ++x) {
                if ((x + y) % 3 != 0) {
                    makeSwamp(x, y);
                }
            }
        }

        for (int y = middleRow + 2; y < bottomPassage - 1; ++y) {
            for (int x = leftWall + 2; x < rightWall - 2; ++x) {
                if ((x + 2 * y) % 4 != 0) {
                    makeSwamp(x, y);
                }
            }
        }

        for (int y = 2; y < topPassage - 1; ++y) {
            for (int x = rightWall + 2; x < width_ - 2; ++x) {
                if ((2 * x + y) % 3 != 0) {
                    makeSwamp(x, y);
                }
            }
        }
    }
}

void World::generateCellStatuses() {
    std::discrete_distribution<int> statusCount({85, 12, 3});
    std::uniform_int_distribution<int> itemChoice(0, 2);

    for (auto& row : field_) {
        for (auto& cell : row) {
            if (!cell.isWalkable()) {
                continue;
            }

            int count = statusCount(generator_);
            if (count == 0) {
                continue;
            }

            std::array<StatusConfig, 5> statuses = {{
                {StatusType::Slowdown, 1, 0},
                {StatusType::Overload, 0, 0},
                {StatusType::Burning, 3, 5},
                {StatusType::Shield, 0, 20},
                {StatusType::ItemPickup, 0, itemChoice(generator_)},
            }};
            std::shuffle(statuses.begin(), statuses.end(), generator_);
            for (int i = 0; i < count; ++i) {
                cell.addStatus(statuses[i]);
            }
        }
    }
}

World::World(int height, int width) : pathFinder_(*this) {
    generator_ = std::mt19937(std::random_device{}());

    height = std::min(maxHeight, height);
    height_ = std::max(minHeight, height);

    width = std::min(maxWidth, width);
    width_ = std::max(minWidth, width);

    field_ = std::vector<std::vector<Cell>>(
        height_, std::vector<Cell>(width_, Cell(true)));

    for (int x = 0; x < width_; ++x) {
        field_[0][x].setMovementCost(impassableMovementCost);
        field_[0][x].setTerrainType(TerrainType::Wall);
        field_[height_ - 1][x].setMovementCost(impassableMovementCost);
        field_[height_ - 1][x].setTerrainType(TerrainType::Wall);
    }

    for (int y = 0; y < height_; ++y) {
        field_[y][0].setMovementCost(impassableMovementCost);
        field_[y][0].setTerrainType(TerrainType::Wall);
        field_[y][width_ - 1].setMovementCost(impassableMovementCost);
        field_[y][width_ - 1].setTerrainType(TerrainType::Wall);
    }

    initDefaultLevel();
    generateCellStatuses();
}

World::~World() {}

int World::getWidth() const { return width_; }

int World::getHeight() const { return height_; }

const Cell& World::getCell(const Point& point) const {
    if (!isInside(point)) {
        throw std::out_of_range("Point is outside");
    }
    return field_[point.getY()][point.getX()];
}

Cell& World::getCell(const Point& point) {
    if (!isInside(point)) {
        throw std::out_of_range("Point is outside");
    }
    return field_[point.getY()][point.getX()];
}

bool World::useAbility(Robot& owner, AbilityType abilityType,
                       std::optional<Point> target) {
    for (auto& status : owner.getAllStatuses()) {
        if (status->blockAbilityAttempt()) {
            owner.clearExpiredStatuses();
            return true;
        }
    }
    const auto& ownerAbilities = owner.getAllAbilities();
    for (auto& ability : ownerAbilities) {
        if (ability->getType() == abilityType) {
            return ability->use(owner, *this, target);
        }
    }
    return false;
}

void World::addStatuses(Robot& target, Cell& cell) {
    auto& statuses = cell.statuses();
    bool pickedUpItem = false;
    for (const StatusConfig& statusConfig : statuses) {
        switch (statusConfig.stausType) {
            case StatusType::Burning:
                target.addPendingStatus(std::make_unique<Burning>(
                    statusConfig.turns, statusConfig.value));
                break;
            case StatusType::Overload:
                target.addPendingStatus(std::make_unique<Overload>());
                break;
            case StatusType::Shield:
                target.addPendingStatus(std::make_unique<Shield>(statusConfig.value));
                break;
            case StatusType::Slowdown:
                target.addPendingStatus(
                    std::make_unique<Slowdown>(statusConfig.turns));
                break;
            case StatusType::ItemPickup:
                if (&target == player_.get()) {
                    ItemPickup pickup(static_cast<ItemType>(statusConfig.value));
                    pickup.apply(*player_);
                    pickedUpItem = true;
                }
                break;

            default:
                break;
        }
    }
    if (pickedUpItem) cell.removeStatus(StatusType::ItemPickup);
    cell.removeRandomStatus(generator_);
}

std::optional<GameEvent> World::popEvent() {
    if (events_.empty()) {
        return std::nullopt;
    }
    GameEvent event = events_.front();
    events_.pop();
    return event;
}

void World::pushEvent(GameEvent event) { events_.push(event); }

void World::addRobot(std::unique_ptr<Robot> robot) {
    if (!robot) {
        return;
    }
    Cell& cell = getCell(robot->getPosition());
    if (cell.isWalkable() && !cell.isTaken()) {
        cell.setOccupantType(OccupantType::Robot);
        robots_.push_back(std::move(robot));
    }
}

bool World::attackRobots(Robot& owner, Robot& target) {
    return attackRobots(owner, target, owner.getAttack());
}

bool World::attackRobots(Robot& owner, Robot& target, int damage) {
    bool killed = owner.attack(target, damage);
    if (killed) {
        killRobot(target);
        grantExp(owner, 50);
    }
    return killed;
}

InteractResult World::interactRobots(Robot& owner, Robot& target) {
    InteractResult result = owner.interact(target);

    switch (result) {
        case InteractResult::Kill: {
            killRobot(target);
            grantExp(owner, 50);
            break;
        }

        default:
            break;
    }

    return result;
}

MoveResult World::moveRobot(Robot& robot, int dx, int dy) {
    for (auto& status : robot.getAllStatuses()) {
        if (status->blocksMovement()) {
            return MoveResult::Moved;
        }
    }

    Point nextPoint(robot.getX() + dx, robot.getY() + dy);

    if (!isInside(nextPoint)) {
        return MoveResult::Blocked;
    }

    Cell& nextCell = getCell(nextPoint);

    if (!nextCell.isWalkable()) {
        return MoveResult::Blocked;
    }

    if (nextCell.getOccupantType() == OccupantType::Robot) {
        auto other = findRobotAt(nextPoint);
        if (other) {
            Robot& target = other->get();

            InteractResult result = interactRobots(robot, target);
            if (result == InteractResult::NotInteracted) {
                return MoveResult::Blocked;
            }
            return MoveResult::Interacted;
        }
    }

    addStatuses(robot, nextCell);

    getCell(robot.getPosition()).clearOccupant();
    nextCell.setOccupantType(OccupantType::Robot);

    robot.setPosition(nextPoint);

    return MoveResult::Moved;
}

bool World::moveRobotWithSpeed(Robot& robot, int dx, int dy) {
    return moveRobotUsingSpeed(robot, [dx, dy](const Robot& movingRobot) {
        return Point(movingRobot.getX() + dx, movingRobot.getY() + dy);
    });
}

bool World::moveRobotUsingSpeed(
    Robot& robot, const std::function<Point(const Robot&)>& selectNextPoint) {
    bool isMoved = false;
    int movementPoints = robot.getSpeed();

    while (movementPoints > 0) {
        const Point nextPoint = selectNextPoint(robot);
        if (nextPoint == robot.getPosition() || !isInside(nextPoint)) {
            break;
        }

        const Cell& nextCell = getCell(nextPoint);
        const int movementCost = nextCell.getMovementCost();
        if (!nextCell.isWalkable() || movementCost > movementPoints ||
            movementCost > robot.getEnergy()) {
            break;
        }

        const int dx = nextPoint.getX() - robot.getX();
        const int dy = nextPoint.getY() - robot.getY();
        const MoveResult moved = moveRobot(robot, dx, dy);
        if (moved == MoveResult::Blocked) {
            break;
        }

        movementPoints -= movementCost;
        robot.expendEnergy(movementCost);
        isMoved = true;

        if (moved == MoveResult::Interacted) {
            break;
        }
    }

    return isMoved;
}

void World::killRobot(Robot& robot) {
    if (&robot == player_.get()) {
        return;
    }
    getCell(robot.getPosition()).clearOccupant();

    std::erase_if(robots_, [&robot](const std::unique_ptr<Robot>& pointer) {
        return pointer.get() == &robot;
    });
}

const std::vector<std::unique_ptr<Robot>>& World::getRobots() const {
    return robots_;
}

std::vector<std::unique_ptr<Robot>>& World::getRobots() { return robots_; }

std::optional<std::reference_wrapper<Robot>> World::findRobotAt(
    const Point& point) {
    if (getPlayer().getPosition() == point) {
        return std::ref(*player_);
    }

    for (auto& robot : robots_) {
        if (robot->getPosition() == point) {
            return std::ref(*robot);
        }
    }
    return std::nullopt;
}

bool World::hasRobotAt(const Point& point) const {
    return isInside(point) &&
           getCell(point).getOccupantType() == OccupantType::Robot;
}

void World::moveBots() {
    if (!player_ || player_->isDead()) {
        return;
    }

    for (auto& robot : getRobots()) {
        if (!robot || robot->isDead()) {
            continue;
        }
        if (robot->getTeam() == Team::Friend) {
            continue;
        }

        pathFinder_.buildDistanceMap(player_->getPosition(),
                                     robot->getPosition());

        auto abilityHeal = robot->getAbility(AbilityType::Heal);
        auto abilityTeleport = robot->getAbility(AbilityType::Teleport);
        bool used = true;
        while (used) {
            if (abilityHeal) {
                Ability& ability = robot->getHealth() > 10 && abilityTeleport
                                       ? abilityTeleport->get()
                                       : abilityHeal->get();
                Point from = robot->getPosition();
                Point target =
                    pathFinder_.getNextPointInRadius(from, ability.range());
                used = useAbility(*robot, ability.getType(), target);
            } else if (abilityTeleport) {
                Ability& ability = *abilityTeleport;
                Point from = robot->getPosition();
                Point target =
                    pathFinder_.getNextPointInRadius(from, ability.range());
                used = useAbility(*robot, ability.getType(), target);
            } else {
                used = false;
            }
        }
        moveRobotUsingSpeed(*robot, [this](const Robot& movingRobot) {
            return pathFinder_.getNextPoint(movingRobot.getPosition());
        });
    }
}

void World::addFactory(std::unique_ptr<Factory> factory, const Point& point) {
    if (!factory) {
        return;
    }

    for (int dy = 0; dy < factory->getHeight(); dy++) {
        for (int dx = 0; dx < factory->getWidth(); dx++) {
            Point cellPoint(point.getX() + dx, point.getY() + dy);

            if (!isInside(cellPoint)) {
                return;
            }

            Cell& currentCell = getCell(cellPoint);
            if (currentCell.isTaken() || !currentCell.isWalkable()) {
                return;
            }
        }
    }
    factory->setPosition(point);

    for (int dy = 0; dy < factory->getHeight(); dy++) {
        for (int dx = 0; dx < factory->getWidth(); dx++) {
            Point cellPoint(point.getX() + dx, point.getY() + dy);
            getCell(cellPoint).setOccupantType(OccupantType::Factory);
        }
    }
    factories_.push_back(std::move(factory));
}

const std::vector<std::unique_ptr<Factory>>& World::getFactories() const {
    return factories_;
}

void World::addPlayer(std::unique_ptr<Player> player) {
    if (!player) {
        throw std::logic_error("Player cannot be nullptr");
    }
    Cell& cell = getCell(player->getPosition());
    if (cell.isWalkable() && !cell.isTaken()) {
        cell.setOccupantType(OccupantType::Robot);
        player_ = std::move(player);
        return;
    }
    throw std::logic_error("Failed to add player");
}

Player& World::getPlayer() {
    if (!player_) {
        throw std::logic_error("World has no player");
    }
    return *player_;
}

const Player& World::getPlayer() const {
    if (!player_) {
        throw std::logic_error("World has no player");
    }
    return *player_;
}

bool World::movePlayer(int dx, int dy) {
    if (!player_ || player_->isDead()) {
        return false;
    }
    Player& player = getPlayer();
    return moveRobotWithSpeed(player, dx, dy);
}

bool World::isInside(const Point& point) const {
    return point.getX() >= 0 && point.getX() < width_ && point.getY() >= 0 &&
           point.getY() < height_;
}

const std::vector<int>& World::getDistanceMap() {
    pathFinder_.buildDistanceMap(getPlayer().getPosition());
    return pathFinder_.getDistanceMap();
}

void World::markSeen() {
    Player& player = getPlayer();
    Point& playerPos = player.getPosition();

    constexpr int directions[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    for (int y = 0; y <= player.getVisibiltyRadius(); y++) {
        for (int x = 0; x <= player.getVisibiltyRadius(); x++) {
            for (auto direction : directions) {
                int newX = playerPos.getX() + x * direction[0];
                int newY = playerPos.getY() + y * direction[1];
                Point currentPoint(newX, newY);
                if (isInside(currentPoint) && player.isInRadius(currentPoint)) {
                    getCell(currentPoint).markSeen();
                }
            }
        }
    }
}

void World::grantExp(Robot& owner, int amount) {
    int upLevels = owner.gainExp(amount);
    GameEvent event;
    event.eventType = EvnetType::LevelUp;
    for (int i = 0; i < upLevels; i++) {
        pushEvent(event);
    }
}

bool World::endTurn() {
    for (auto& robot : robots_) {
        if (robot) {
            robot->onTurnEnd();
        }
    }
    std::erase_if(robots_, [this](const auto& robot) {
        if (robot && robot->isDead()) {
            getCell(robot->getPosition()).clearOccupant();
            return true;
        }
        return false;
    });

    for (auto& factory : factories_) {
        if (factory) {
            Point spawnPoint(factory->getX() - 1, factory->getY());
            if (isInside(spawnPoint) && !getCell(spawnPoint).isTaken()) {
                auto newRobot = factory->spawn(generator_);
                if (newRobot) {
                    newRobot->setPosition(spawnPoint);
                    addRobot(std::move(newRobot));
                }
            }
        }
        factory->onTurnEnd();
    }

    if (player_) {
        player_->onTurnEnd();
    }
    const bool hasEnemies =
        std::any_of(robots_.begin(), robots_.end(), [](const auto& robot) {
            return robot && !robot->isDead() && robot->getTeam() == Team::Enemy;
        });
    if (!hasEnemies || getPlayer().isDead()) {
        return true;
    }
    return false;
}
