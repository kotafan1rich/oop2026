#pragma once

#include <optional>

#include "game/menu/AbilityMenu.hpp"
#include "game/world/World.hpp"
#include "terminal/Input.hpp"
#include "terminal/Renderer.hpp"
#include "terminal/Terminal.hpp"

class Game {
   private:
    Terminal terminal_;
    Renderer renderer_;
    Input input_;
    World world_;

    AbilityMenu abilityMenu_;
    BacklightMode backlitingMode_ = BacklightMode::Default;

   public:
    Game(int height, int width);
    ~Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void Run();
    bool processCommand(const Command& command);

    bool useAbility();
    void chooseAbility();
    void addAbility(AbilityType abilityType);
    std::unique_ptr<Ability> createAbility(AbilityType abilityType) const;

    std::optional<Point> choosePointInRadius(int radius);

    void nextBacklightingMode();

    void handleGameEvents();
};
