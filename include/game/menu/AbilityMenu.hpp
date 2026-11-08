#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "game/abilities/Ability.hpp"
#include "terminal/Input.hpp"
#include "terminal/Terminal.hpp"

const std::string splashText = "Удар вокруг";
const std::string rangedAttackText = "Дальний удар";
const std::string healText = "Лечение";
const std::string teleportText = "Телепортация";

constexpr std::string_view abilityChoiceText =
    "Выбери способность:\n"
    "1. Удар вокруг\n"
    "2. Дальний удар\n"
    "3. Лечение\n"
    "4. Телепортация\n";

class AbilityMenu {
   private:
    Terminal& terminal_;
    Input& input_;

   public:
    AbilityMenu(Terminal& terminal, Input& input);

    std::string buildMenu(std::vector<AbilityType> available);

    AbilityType choose();
    AbilityType choose(std::vector<AbilityType> available);
};
