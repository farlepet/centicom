#include <iostream>
#include <sstream>
#include <string>

#include <signal.h>

#include <Curses.hpp>

#define CTRL(x) ((x) & 0x1F)

Curses::Curses() {

}

void Curses::start() {
    this->rootWind = initscr();
    if(this->rootWind == nullptr) {
        this->endWithError("Could not initialize root window!");
    }

    this->termWin = derwin(this->rootWind, 0, 0, 0, 0);
    if(this->termWin == nullptr) {
        this->endWithError("Could not create terminal sub-window!");
    }

    if(this->cmdWinHeight) {
        this->showCommandWindow();
    }

    raw();
    keypad(this->rootWind, TRUE);
    noecho();
}

void Curses::end() {
    endwin();
}

void Curses::endWithError(std::string str) {
    this->end();
    throw std::runtime_error("Curses error: " + str);
}

void Curses::showCommandWindow(void) {
    int width, height;
    getmaxyx(this->rootWind, height, width);

    int cmdWinRow = (height - this->cmdWinHeight);

    wresize(this->termWin, cmdWinRow, width);

    this->cmdWin = derwin(this->rootWind, this->cmdWinHeight, 0, height - this->cmdWinHeight, 0);
    if(this->termWin == nullptr) {
        this->endWithError("Could not create command sub-window!");
    }
    box(this->cmdWin, 0, 0);
    scrollok(this->cmdWin, TRUE);
}

void Curses::hideCommandWindow(void) {
    int width, height;
    getmaxyx(this->rootWind, height, width);

    delwin(this->cmdWin);
    this->cmdWin = nullptr;

    wresize(this->termWin, height, width);
}

std::string Curses::getLine(void) {
    wclear(this->cmdWin);
    box(this->cmdWin, 0, 0);
    wmove(this->cmdWin, 1, 1);
    wrefresh(this->cmdWin);

    std::stringstream ss;

    do {
        int ch = getch();

        if(ch == KEY_ENTER || ch == '\n') {
            /* @todo Support multi-line buffering */
            /* @todo Support multiple types of line endings */
            ss << "\r\n";
            wclear(this->cmdWin);
            wrefresh(this->cmdWin);
            break;
        } else if(ch == KEY_BACKSPACE || ch == '\b') {
            /* Likely a more effecient way to do this. */
            std::string tstr = ss.str();
            if(tstr.size()) {
                ss.str(tstr.substr(0, tstr.size()-1));

                int x, y;
                getyx(this->cmdWin, y, x);
                x--;
                mvwaddch(this->cmdWin, y, x, ' ');
                wmove(this->cmdWin, y, x);
            }
        } else if (ch == CTRL('c')) {
            raise(SIGINT);
        } else {
            wprintw(this->cmdWin, "%c", ch);
            ss << (char)ch;
        }

        wrefresh(this->cmdWin);

    } while(1);

    return ss.str();
}

void Curses::writeTerminal(std::string str) {
    wprintw(this->termWin, "%s", str.c_str());
    wrefresh(this->termWin);
}

void Curses::putTerminal(uint8_t ch) {
    wprintw(this->termWin, "%c", ch);
    wrefresh(this->termWin);
}