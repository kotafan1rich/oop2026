cmake_minimum_required(VERSION 3.20)

project(oop2026)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(game
    src/main.cpp

    src/game/Game.cpp
    src/game/abilities/Heal.cpp
    src/game/abilities/RangedAttach.cpp
    src/game/abilities/Splash.cpp
    src/game/abilities/Teleport.cpp
    src/game/navigation/PathFinder.cpp
    src/game/units/Player.cpp
    src/game/units/Robot.cpp
    src/game/world/Cell.cpp
    src/game/world/Factory.cpp
    src/game/world/Point.cpp
    src/game/world/World.cpp
    src/game/menu/AbilityMenu.cpp

    src/terminal/Input.cpp
    src/terminal/Renderer.cpp
    src/terminal/Terminal.cpp
)

target_include_directories(game PRIVATE include)
