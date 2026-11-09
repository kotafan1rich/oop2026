#include "terminal/Input.hpp"

Input::Input(Terminal& terminal) : terminal_(terminal) {}

Input::~Input() {}

Command Input::getCommand() const {
    char c = terminal_.readChar();
    switch (c) {
        case '\0':
            return Command::Quit;

        case 'w':
            return Command::Up;

        case 's':
            return Command::Down;

        case 'a':
            return Command::Left;

        case 'd':
            return Command::Right;

        case 'e':
            return Command::Use;

        case '1':
            return Command::UseHealthItem;
        case '2':
            return Command::UseEnergyItem;
        case '3':
            return Command::UseAttackItem;

        case '\n':
            return Command::Enter;

        case 'r':
        case ' ':
            return Command::Wait;

        case '\t':
            return Command::NextBacklightingMode;

        case 'q':
            return Command::Quit;

        default:
            return Command::None;
    }
}

Choice Input::getChoice() const {
    char c = terminal_.readChar();
    switch (c) {
        case '1':
            return Choice::One;
        case '2':
            return Choice::Two;
        case '3':
            return Choice::Three;
        case '4':
            return Choice::Four;
        case 'q':
            return Choice::Quit;
        default:
            return Choice::None;
    }
}
