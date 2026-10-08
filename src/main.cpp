#include <exception>
#include <iostream>

#include "game/Game.hpp"

int main() {
    try {
        Game game(30, 30);

        game.Run();
        return 0;
    } catch (const std::exception& error) {
        std::cout << "Fatal error: " << error.what() << '\n';
        return 1;
    }
}