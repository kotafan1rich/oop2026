#include "game/Game.hpp"

#include "game/units/Player.hpp"
#include "game/units/Robot.hpp"
#include "game/world/Factory.hpp"
#include "game/world/World.hpp"

Game::Game(int height, int width)
    : terminal_(), renderer_(), input_(terminal_), world_(height, width) {
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

    Robot robotTemplate;
    robotTemplate.setAttack(5);
    robotTemplate.setSpeed(1);

    auto factory = std::make_unique<Factory>(robotTemplate);
    factory->setSpawnInterval(10);

    world_.addFactory(std::move(factory), Point(27, 27));
}

Game::~Game() {}

void Game::Run() {
    while (true) {
        world_.markSeen();
        terminal_.draw(renderer_.render(world_));

        Command command = input_.getCommand();
        if (command == Command::Quit) break;

        if (processCommand(command)) {
            world_.moveBots();

            if (world_.endTurn()) {
                terminal_.draw(renderer_.render(world_));
                break;
            }
        }
    }
}

bool Game::processCommand(const Command& command) {
    switch (command) {
        case Command::Up:
            return world_.movePlayer(0, -1);

        case Command::Right:
            return world_.movePlayer(1, 0);

        case Command::Down:
            return world_.movePlayer(0, 1);

        case Command::Left:
            return world_.movePlayer(-1, 0);

        case Command::Wait:
            return true;

        default:
            return false;
    }
}
