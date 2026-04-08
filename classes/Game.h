#pragma once
#include "../classes/IBuilder.h"
#include "../classes/DCList.h"

class Game : public IBuilder {
private:
    enum Role
    {
        TAPPER,
        GOOSE,
        IT
    };

    struct Player {
        std::string name;
        Role status;
        int playerID; //index in list; no method for setting id is needed as it is set upon creation and removal
        bool isEliminated = false;
    };

    unsigned int round;
    DCList circle;
    std::vector<Player> cookingPot;
    int itIndex; // tracks who is "it" in the circle

public:
    Game();
    ~Game();

    void buildMenu();

    Player createPlayer(std::string name);

    const Player& getPlayer(int id) const;

    void setRole(Player& player, Role role);

    void createCircle();

    void renderCircle() const;

    void addToPot(const Player& player);

    void renderPot() const;

    // main game loop
    void play();
};