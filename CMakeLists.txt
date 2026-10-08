cmake_minimum_required(VERSION 3.20)

project(oop2026)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(game
    src/main.cpp

    src/game/Game.cpp
    src/game/World.cpp
    src/game/Cell.cpp
    src/game/Robot.cpp
    src/game/Player.cpp
    src/game/Point.cpp
    src/game/Renderer.cpp
    src/game/PathFinder.cpp
    src/game/Factory.cpp

    src/terminal/Terminal.cpp
    src/terminal/Input.cpp
)

target_include_directories(game PRIVATE include)
