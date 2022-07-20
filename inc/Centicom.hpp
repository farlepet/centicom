#include <thread>

#include <Curses.hpp>
#include <Connection.hpp>

struct CenticomSettings {
    ConnectionSettings cSettings;
};

class Centicom {
private:
    CenticomSettings settings;

    Curses     curses;
    Connection conn;

    bool running = 0;

    void commThreadFunc(void);

    void termThreadFunc(void);

public:
    Centicom(CenticomSettings &_settings);

    /**
     * @brief Start Centicom
     */
    void run(void);

    /**
     * @brief Stop Centicom
     */
    void stop(void);
};
