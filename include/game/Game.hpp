#pragma once

#include "game/world/World.hpp"
#include "terminal/Renderer.hpp"
#include "terminal/Terminal.hpp"

#include "terminal/Input.hpp"

class Game {
   private:
    Terminal terminal_;
    Renderer renderer_;
    Input input_;
    World world_;

   public:
    Game(int height, int width);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void Run();
    bool processCommand(const Command& command);
};
