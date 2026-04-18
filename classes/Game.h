#pragma once
#include "../classes/DCList.h"
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_audio.h>

class Game{
private:
    typedef struct Sprite {
        SDL_Texture* texture = nullptr;
        int w;
        int h;
    };

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


    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    SDL_AudioDeviceID audioDevice = 0;

    static constexpr int screenWidth = 800;
    static constexpr int screenHeight = 600;

    Sprite loadSprite(const char* path, int w, int h);

    bool initSDL();     // init window, renderer, audio
    void shutdownSDL(); // cleanup
    void handleEvents(SDL_Event& e, bool& running);
    void render();      // your draw calls go here

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