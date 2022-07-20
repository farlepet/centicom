#include <string>

#include <ncurses.h>

/**
 * @brief Handles ncurses side of terminal
 */
class Curses {
private:
    /** Root window */
    WINDOW *rootWind;
    /** Terminal window */
    WINDOW *termWin;
    /** Command window */
    WINDOW *cmdWin;

    /** Command window height, not including any border. */
    int cmdWinHeight = 4;

    /**
     * @brief Clean up ncurses, and throw an exception.
     *
     * @param str Details to include in exception
     */
    void endWithError(std::string str);

public:
    /**
     * @brief Constructor, currently does nothing special
     */
    Curses();

    /**
     * @brief Setup ncurses
     */
    void start(void);

    /**
     * @brief Tear down ncurses
     */
    void end(void);

    /**
     * @brief Enable command entry window
     */
    void showCommandWindow(void);

    /**
     * @brief Disable command entry window
     */
    void hideCommandWindow(void);

    /**
     * @brief Get line from the command entry window
     *
     * @note May be switched to a more raw storage object to allow for special
     * characters.
     *
     * @return std::string Character data from line
     */
    std::string getLine(void);

    /**
     * @brief Write data to the terminal window
     *
     * @param str Character data to write
     */
    void writeTerminal(std::string str);

    /**
     * @brief Write single character to terminal window
     *
     * @param ch Character to write
     */
    void putTerminal(uint8_t ch);
};
