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
    std::vector<std::string> menuOptions;
    std::vector<std::function<void()>> menuFunctions;
    HANDLE hConsole;  // add this

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

    void push_toOptions(const std::vector<std::string>&);
    void push_Functions(std::function<void()>);
    void push_Functions(const std::vector<std::function<void()>>& f);
    void buildMenu();

    IBuilder();
    IBuilder(bool);
    IBuilder(std::vector<std::string>);
};