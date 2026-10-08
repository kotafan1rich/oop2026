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

        case 'r':
        case ' ':
            return Command::Wait;

        case 'q':
            return Command::Quit;
        default:
            return Command::None;
    }
}
