#pragma once

#include <termios.h>

#include <string_view>

constexpr std::string_view hideCursor = "\033[?25l";
constexpr std::string_view showCursor = "\033[?25h";
constexpr std::string_view startCursor = "\033[H";
constexpr std::string_view clearToScreenEnd = "\033[J";

class Terminal {
   private:
    termios oldTerminal{};
    int oldFlags{};

   public:
    Terminal();
    ~Terminal();

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    char readChar() const;
    void draw(std::string_view out);
};
