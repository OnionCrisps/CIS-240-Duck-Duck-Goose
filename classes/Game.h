#pragma once
#include "../classes/DCList.h"
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

class Game {
private:
    enum Role { DUCK, GOOSE, IT };

    enum class GameState {
        SPLASH, 
        MAIN_MENU, 
        OPTIONS, 
        PLAYER_SETUP,
        PLAYING, 
        PAUSED, 
        ROUND_END, 
        GAME_OVER
    };
    enum class PlayPhase {
        CHOOSING_DIRECTION,
        WALKING,
        CHASING
    };
    PlayPhase playPhase = PlayPhase::CHOOSING_DIRECTION;

    struct Sprite {
        SDL_Texture* texture = nullptr;
        int src_width = 0;
        int src_height = 0;
        float scaleFactor = 1.0f;
    };

    struct Player {
        std::string name;
        Sprite roleSprite;
        Role status = DUCK;
        int playerID = 0;
        bool isEliminated = false;
    };

    struct Button {
        Sprite normal;
        Sprite hover;
        SDL_Rect rect;      // position and size in base coords
        bool isHovered = false;
    };

    struct Assets {
        Sprite tapperSprite;
        Sprite gooseSprite;
        Sprite itSprite;
        Sprite bgTile;
        // splash
        Sprite splashTitle;
        Sprite start;

        //main menu 
        Sprite mainMenuTitle;

        //main menu buttons
        Button playButton;
        Button optionsButton;
        Button quitButton;
    } assets;

   

    std::vector<SDL_Texture*> registeredTextures;

    // game data
    DCList circle;
    std::vector<Player> activePlayers; // master list, parallel to circle
    std::vector<Player> cookingPot;
    // gameplay state
    unsigned int round = 0;
    int        itIndex = 0;       // who is IT in the circle
    int        tapperIndex = 0;       // IT's current position walking the circle
    bool       goingClockwise = true;  // direction IT is walking
    int        selectedIndex = 0;      // which player IT is hovering over

    // tap meter
    float tapEnergy = 0.0f;   // current fill 0.0 - 1.0
    float targetPos = 0.5f;   // where the bar is (0.0 - 1.0)
    float meterDecay = 0.02f;  // how fast energy drains per frame
    float meterFill = 0.15f;  // how much each tap adds

    int   barChangeCount = 0;      // how many times bar has moved
    int   maxBarChanges = 8;      // scales with player count
    bool  chaseActive = false;  // goose is currently chasing

    // round
    int gooseIndex = -1;            // who got called goose

    // state
    GameState state = GameState::SPLASH;
    bool needstoRedraw = true;
    int  menuSelection = 0;

    // SDL
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_AudioDeviceID audioDevice = 0;

    // base resolution — logical size, never changes
    static constexpr int baseWidth = 800;
    static constexpr int baseHeight = 600;

    Uint32 fadeTimer = 0;
    float  fadeAlpha = 255.0f;
    float  fadeSpeed = 150.0f; // alpha units per second
    bool   fadingOut = true;

    // SDL lifecycle and helpers
    bool initSDL();
    void initAssets();
    void getLogicalMouse(int&, int&);

    void shutdownSDL();

    // sprites and buttons
    Sprite loadSprite(const char* path, float scale = 1.0f);
    void renderButton(const Button& btn);
    bool isHovered(const SDL_Rect& rect, int mx, int my);
    // renderers
    void renderSplash();
    void renderMM();
    void renderOptions();
    void renderSetup();
    void renderGame();
    void renderTapMeter();
    void renderPause();
    void renderRoundEnd();
    void renderGameOver();

    // events
    void handleEvents(SDL_Event& e, bool& running);
    void handleMenuEvents(SDL_Event& e, bool& running);
    void handleOptionsEvents(SDL_Event& e, bool& running);
    void handleSetupEvents(SDL_Event& e, bool& running);
    void handleGameEvents(SDL_Event& e, bool& running);
    void handlePauseEvents(SDL_Event& e, bool& running);

    // game logic
    void updatePos();
    void renderPlayer(const Player& player, int x, int y);

    Player createPlayer(std::string name);
    const Player& getPlayer(int id) const;
    void setRole(Player& player, Role role);
    void createCircle();
    void addToPot(const Player& player);

public:
    Game();
    ~Game();
    void play();


};