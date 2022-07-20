#include <iostream>

#include <Centicom.hpp>

Centicom::Centicom(CenticomSettings &_settings) :
conn(_settings.cSettings) {
    this->settings = _settings;
}

void Centicom::run(void) {
    try {
        this->curses.start();
        this->conn.connect();

        this->running = true;

        std::thread connThread(&Centicom::commThreadFunc, this);
        std::thread termThread(&Centicom::termThreadFunc, this);

        connThread.join();
        termThread.join();
    } catch (std::runtime_error &e) {
        this->curses.end();
        throw e;
    }
}

void Centicom::stop(void) {
    this->curses.end();
}

void Centicom::commThreadFunc(void) {
    while(this->running) {
        int ch = this->conn.getChar();
        if(ch >= 0) {
            /* @todo: Create byte-wise access to terminal */
            this->curses.putTerminal(ch);
        }
    }
}

void Centicom::termThreadFunc(void) {
    while(this->running) {
        std::string line = this->curses.getLine();
        for(size_t i = 0; i < line.size(); i++) {
            this->conn.putChar(line[i]);
        }
    }
}
