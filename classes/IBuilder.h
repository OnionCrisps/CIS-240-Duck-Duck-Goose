#pragma once
#include <windows.h>
#include <vector>
#include <string>
#include <functional>

#define UP_ARROW    72
#define DOWN_ARROW  80
#define ENTER_KEY   13

class IBuilder {
private:
    
    /**
     * @brief Holds the strings for the interface options.
     */
    std::vector<std::string> options;
    
    /**
     * @brief Holds the functions for the interface as they correspond to each option.
     */
    std::vector<std::function<void()>> functions;

    /**
     * @brief A Windows console handle (HANDLE) that represents an opaque handle to a console input or output buffer used with Win32 console APIs.
     */
    HANDLE hConsole;

    bool isCentered = true;

    void clearScreen();
    int _get_len_FromOptions() const;

public:
    void displayMenu(const std::vector<std::string>& menuData, int selection, int w, int h);
    void setCursorPosition(int x, int y);
    void setCentering(bool);
    void s_TxtColor(WORD attributes);

    int getConsoleWidth() const;
    int getConsoleHeight() const;

    void pushToOptions(const std::vector<std::string>&);
    void pushToFunctions(std::function<void()>);
    void pushToFunctions(const std::vector<std::function<void()>>& f);
    void buildMenu();

    IBuilder();
    IBuilder(bool);
    IBuilder(std::vector<std::string>);
};