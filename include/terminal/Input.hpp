#pragma once

#include "terminal/Terminal.hpp"

enum class Command {
    Up,
    Down,
    Right,
    Left,
    Wait,
    Quit,
    None,
};

class Input {
   private:
    Terminal& terminal_;

   public:
    explicit Input(Terminal& terminal);
    ~Input();

    Command getCommand() const;
};
