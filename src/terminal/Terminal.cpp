#include "terminal/Terminal.hpp"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>

Terminal::Terminal() {
    tcgetattr(STDIN_FILENO, &oldTerminal);

    termios newTerminal = oldTerminal;

    newTerminal.c_lflag &= ~(ICANON | ECHO);

    newTerminal.c_cc[VMIN] = 1;
    newTerminal.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newTerminal);

    oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, oldFlags);

    std::cout << "\033[2J";
    std::cout << hideCursor;
}

Terminal::~Terminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTerminal);
    fcntl(STDIN_FILENO, F_SETFL, oldFlags);

    std::cout << showCursor;
}

char Terminal::readChar() const {
    char c;
    if (read(STDIN_FILENO, &c, 1) != 1) {
        c = '\0';
    }
    return c;
}

void Terminal::draw(std::string_view out) {
    std::cout << startCursor << clearToScreenEnd << out << std::flush;
}
