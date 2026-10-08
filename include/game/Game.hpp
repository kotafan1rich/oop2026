#pragma once

#include "terminal/Input.hpp"
#include <game/Renderer.hpp>
#include <game/Robot.hpp>
#include <game/World.hpp>
#include <terminal/Terminal.hpp>
#include <vector>

class Game
{
  private:
    Terminal terminal_;
    Renderer renderer_;
    Input input_;
    World world_;

  public:
    Game(int height, int width);
    ~Game();

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    void Run();
    bool processCommand(const Command &command);
};
