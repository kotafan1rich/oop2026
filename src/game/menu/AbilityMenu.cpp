#include "game/menu/AbilityMenu.hpp"

#include <string>
#include <vector>

AbilityMenu::AbilityMenu(Terminal& terminal, Input& input)
    : terminal_(terminal), input_(input) {}

std::string AbilityMenu::buildMenu(std::vector<AbilityType> abilities) {
    std::string result = "Выбери способность:\n";
    int i = 1;
    for (AbilityType ability : abilities) {
        switch (ability) {
            case AbilityType::Heal:
                result += std::to_string(i) + ": " + healText + "\n";
                break;
            case AbilityType::RangedAttack:
                result += std::to_string(i) + ": " + rangedAttackText + "\n";
                break;
            case AbilityType::Splash:
                result += std::to_string(i) + ": " + splashText + "\n";
                break;
            case AbilityType::Teleport:
                result += std::to_string(i) + ": " + teleportText + "\n";
                break;

            default:
                break;
        }
        i++;
    }
    return result;
}

AbilityType AbilityMenu::choose() {
    terminal_.draw(abilityChoiceText);

    Choice choice = input_.getChoice();
    while (choice == Choice::None || choice == Choice::Quit) {
        terminal_.draw(abilityChoiceText);
        choice = input_.getChoice();
    }

    switch (choice) {
        case Choice::One:
            return AbilityType::Splash;
        case Choice::Two:
            return AbilityType::RangedAttack;
        case Choice::Three:
            return AbilityType::Heal;
        case Choice::Four:
            return AbilityType::Teleport;
    }
    return AbilityType::Splash;
}

AbilityType AbilityMenu::choose(std::vector<AbilityType> available) {
    terminal_.draw(buildMenu(available));
    Choice choice = input_.getChoice();
    while (choice == Choice::None || choice == Choice::Quit) {
        terminal_.draw(buildMenu(available));
        choice = input_.getChoice();
    }

    int i = 1;
    switch (choice) {
        case Choice::One:
            i = 0;
            break;
        case Choice::Two:
            i = 1;
            break;
        case Choice::Three:
            i = 2;
            break;
        case Choice::Four:
            i = 3;
            break;
    }

    if (i >= 0 && i < available.size()) {
        return available[i];
    }
    return available[0];
}
