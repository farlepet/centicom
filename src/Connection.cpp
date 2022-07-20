#include <iostream>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <Connection.hpp>

Connection::Connection(ConnectionSettings &_settings) {
    this->settings = _settings;
}

void Connection::connect(void) {
    this->portFd = open(this->settings.portPath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(this->portFd == -1) {
        throw std::runtime_error(std::string("Could not open TTY `") + this->settings.portPath + "`: " + strerror(errno));
    }
}

static speed_t _getStdBaudRate(uint32_t baud) {
    /* @todo Support non-standard baud rates */
    switch(baud) {
        case 50:      return B50;
        case 75:      return B75;
        case 110:     return B110;
        case 134:     return B134;
        case 150:     return B150;
        case 200:     return B200;
        case 300:     return B300;
        case 600:     return B600;
        case 1200:    return B1200;
        case 1800:    return B1800;
        case 2400:    return B2400;
        case 4800:    return B4800;
        case 9600:    return B9600;
        case 19200:   return B19200;
        case 38400:   return B38400;
        case 57600:   return B57600;
        case 115200:  return B115200;
        case 230400:  return B230400;
        case 460800:  return B460800;
        case 500000:  return B500000;
        case 576000:  return B576000;
        case 921600:  return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;

        default:      return 0;
    }
}

void Connection::configure(void) {
    struct termios connSettings;
    if(tcgetattr(this->portFd, &connSettings)) {
        throw std::runtime_error(std::string("Could not get port attributes: ") + strerror(errno));
    }
    speed_t speed = _getStdBaudRate(this->settings.baud);
    if(speed == 0) {
        throw std::runtime_error("Unsupported baud rate: " + this->settings.baud);
    }
    cfsetispeed(&connSettings, speed);
    cfsetospeed(&connSettings, speed);

    /* @todo Allow configuring these options */
    connSettings.c_cflag &= ~PARENB; /* Parity */
    connSettings.c_cflag &= ~CSTOPB; /* Stop bits */
    connSettings.c_cflag = (connSettings.c_cflag & ~CSIZE) | /* Data bits */
                           CS8;
    
    if(tcsetattr(this->portFd, TCSANOW, &connSettings)) {
        throw std::runtime_error(std::string("Could not set port attributes: ") + strerror(errno));
    }
}

int Connection::getChar(void) {
    uint8_t ch;
    if(read(this->portFd, &ch, 1) != 1) {
        return -1;
    }
    return (int)ch;
}

int Connection::putChar(uint8_t ch) {
    if(write(this->portFd, &ch, 1) != 1) {
        return -1;
    }
    return 0;
}
