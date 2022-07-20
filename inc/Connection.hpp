#include <stdint.h>
#include <string>

struct ConnectionSettings {
    std::string portPath;

    uint32_t baud;
};

class Connection {
private:
    ConnectionSettings settings;

    int portFd;

public:
    Connection(ConnectionSettings &_settings);

    void connect(void);

    void configure(void);

    int getChar(void);

    int putChar(uint8_t ch);
};