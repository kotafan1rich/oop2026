#include "game/Game.hpp"

#include "game/abilities/Heal.hpp"
#include "game/abilities/RangedAttack.hpp"
#include "game/abilities/Splash.hpp"
#include "game/abilities/Teleport.hpp"
#include "game/units/Player.hpp"
#include "game/units/Robot.hpp"
#include "game/world/Factory.hpp"
#include "game/world/World.hpp"
#include "terminal/Renderer.hpp"

Game::Game(int height, int width)
    : terminal_(),
      renderer_(),
      input_(terminal_),
      world_(height, width),
      abilityMenu_(terminal_, input_) {
    auto player = std::make_unique<Player>();
    auto robot1 = std::make_unique<Robot>();
    auto robot2 = std::make_unique<Robot>();
    auto friendlyRobot1 = std::make_unique<Robot>();
    auto friendlyRobot2 = std::make_unique<Robot>();
    auto friendlyRobot3 = std::make_unique<Robot>();

    Point playerPoint = Point(1, 1);
    player->setPosition(playerPoint);
    player->setAttack(20);

    Point robotPoint1 = Point(16, 16);
    robot1->setPosition(robotPoint1);

    Point robotPoint2 = Point(11, 16);
    robot2->setPosition(robotPoint2);

    friendlyRobot1->setTeam(Team::Friend);
    friendlyRobot1->setPosition(Point(3, 1));

    friendlyRobot2->setTeam(Team::Friend);
    friendlyRobot2->setPosition(Point(15, 10));

    friendlyRobot3->setTeam(Team::Friend);
    friendlyRobot3->setPosition(Point(24, 24));

    world_.addPlayer(std::move(player));
    world_.addRobot(std::move(robot1));
    world_.addRobot(std::move(robot2));
    world_.addRobot(std::move(friendlyRobot1));
    world_.addRobot(std::move(friendlyRobot2));
    world_.addRobot(std::move(friendlyRobot3));

    RobotConfig factoryRobot;
    factoryRobot.maxHealth = 70;

    auto factory = std::make_unique<Factory>(factoryRobot);
    factory->setSpawnInterval(20);

    world_.addFactory(std::move(factory), Point(27, 27));
}

Game::~Game() {}

void Game::Run() {
    chooseAbility();

    while (true) {
        world_.markSeen();
        renderer_.drawWorld(world_, backlitingMode_);
        terminal_.draw(renderer_.renderWorld(world_, backlitingMode_));

        Command command = input_.getCommand();
        if (command == Command::Quit) break;

        if (processCommand(command)) {
            world_.moveBots();

            if (world_.endTurn()) {
                renderer_.drawWorld(world_, backlitingMode_);
                terminal_.draw(renderer_.renderWorld(world_, backlitingMode_));
                break;
            }
        }
    }
}

bool Game::processCommand(const Command& command) {
    bool processed = false;
    switch (command) {
        case Command::Up:
            processed = world_.movePlayer(0, -1);
            break;

        case Command::Right:
            processed = world_.movePlayer(1, 0);
            break;

        case Command::Down:
            processed = world_.movePlayer(0, 1);
            break;

        case Command::Left:
            processed = world_.movePlayer(-1, 0);
            break;

        case Command::Wait:
            processed = true;
            break;

        case Command::Use:
            useAbility();
            processed = false;
            break;

        case Command::NextBacklightingMode:
            nextBacklightingMode();
            processed = false;
            break;

        case Command::UseHealthItem:
            world_.getPlayer().useItem(ItemType::Health);
            break;
        case Command::UseEnergyItem:
            world_.getPlayer().useItem(ItemType::Energy);
            break;
        case Command::UseAttackItem:
            world_.getPlayer().useItem(ItemType::Attack);
            break;
    }
    handleGameEvents();
    return processed;
}

bool Game::useAbility() {
    AbilityType abilityType =
        abilityMenu_.choose(world_.getPlayer().getAllAbilityTypes());

    switch (abilityType) {
        case AbilityType::Heal:
        case AbilityType::Splash:
            return world_.useAbility(world_.getPlayer(), abilityType,
                                     std::nullopt);

        case AbilityType::RangedAttack:
        case AbilityType::Teleport: {
            auto ability = world_.getPlayer().getAbility(abilityType);
            if (!ability) {
                break;
            }
            std::optional<Point> target =
                choosePointInRadius(ability->get().range());
            return world_.useAbility(world_.getPlayer(), abilityType, target);
        }
        default:
            break;
    }
    return false;
}

std::optional<Point> Game::choosePointInRadius(int radius) {
    Point point = world_.getPlayer().getPosition();
    Point center = point;
    renderer_.drawWorld(world_, backlitingMode_);
    renderer_.highlightCircle(world_, center, radius + 1);
    renderer_.highlightPoint(world_, point);
    terminal_.draw(renderer_.renderWorld(world_, backlitingMode_));

    Command command = input_.getCommand();
    while (command != Command::Enter) {
        int newX = point.getX();
        int newY = point.getY();

        switch (command) {
            case Command::Down:
                newY++;
                break;
            case Command::Left:
                newX--;
                break;
            case Command::Up:
                newY--;
                break;
            case Command::Right:
                newX++;
                break;
            case Command::Quit:
                return std::nullopt;
            case Command::NextBacklightingMode:
                nextBacklightingMode();
                renderer_.drawWorld(world_, backlitingMode_);
                renderer_.highlightCircle(world_, center, radius + 1);
                renderer_.highlightPoint(world_, point);
                terminal_.draw(renderer_.renderWorld(world_, backlitingMode_));
                command = input_.getCommand();
                continue;
            default:
                command = input_.getCommand();
                continue;
        }

        Point newPoint(newX, newY);
        if (world_.isInside(newPoint) && center.dist(newPoint) <= radius) {
            point = newPoint;
            renderer_.drawWorld(world_, backlitingMode_);
            renderer_.highlightCircle(world_, center, radius + 1);
            renderer_.highlightPoint(world_, point);
            terminal_.draw(renderer_.renderWorld(world_, backlitingMode_));
        }
        command = input_.getCommand();
    }
    return point;
}

void Game::nextBacklightingMode() {
    int next = (static_cast<int>(backlitingMode_) + 1) %
               static_cast<int>(BacklightMode::Count_);

    backlitingMode_ = static_cast<BacklightMode>(next);
}

void Game::handleGameEvents() {
    std::optional<GameEvent> event = world_.popEvent();
    while (event) {
        switch (event->eventType) {
            case EvnetType::LevelUp:
                chooseAbility();
                break;

            default:
                break;
        }
        event = world_.popEvent();
    }
}

void Game::chooseAbility() {
    Player& player = world_.getPlayer();
    AbilityType abilityType = abilityMenu_.choose();

    auto ability = player.getAbility(abilityType);
    if (ability) {
        player.upgradeAbility(abilityType);
    } else {
        addAbility(abilityType);
    }
}

void Game::addAbility(AbilityType abilityType) {
    world_.getPlayer().addAbility(createAbility(abilityType));
}

std::unique_ptr<Ability> Game::createAbility(AbilityType abilityType) const {
    switch (abilityType) {
        case AbilityType::Heal:
            return std::make_unique<Heal>();
        case AbilityType::Splash:
            return std::make_unique<Splash>();
        case AbilityType::RangedAttack: {
            auto ability = std::make_unique<RangedAttack>();
            ability->addAppliedStatus(rangedBurningDefault);
            return ability;
        }
        case AbilityType::Teleport:
            return std::make_unique<Teleport>();
    }
    throw std::invalid_argument("Unknown ability type");
}
