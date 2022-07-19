#include <iostream>

#include <Curses.hpp>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    std::cout << "centicom" << std::endl;

    Curses curses;
    curses.start();
    curses.end();
    return 0;
}
