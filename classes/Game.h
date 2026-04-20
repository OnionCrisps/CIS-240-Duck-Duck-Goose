#pragma once
#include "../classes/DCList.h"
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

/*Gavin Harban, Jon Moore, Game.h CIS 240-1 Data Structures Project*/
class Game {
private:
    enum Role { DUCK, GOOSE, IT };
    // callout popup
    enum class CalloutType { NONE, DUCK, GOOSE };
    CalloutType activeCallout = CalloutType::NONE;
    Uint32      calloutTimer = 0;

    static constexpr Uint32 CALLOUT_DURATION = 1200; // ms to show callout
    int calloutTargetX = 0; // screen position to show it near
    int calloutTargetY = 0;

    enum class GameState {
        SPLASH, 
        MAIN_MENU, 
        OPTIONS, 
        PLAYER_SETUP,
        CONFIGURE,
        PLAYING, 
        PAUSED, 
        ROUND_END, 
        GAME_OVER
    };
    enum class PlayPhase {
        SPINNING,           // roulette highlighting
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
    struct Animation {
        SDL_Texture* sheet = nullptr;
        int frameW = 0;
        int frameH = 0;
        int frameCount = 0;
        int currentFrame = 0;
        Uint32 lastFrame = 0;
        Uint32 frameDelay = 120;
        int row = 0;

    };

    struct Player {
        std::string name;
        Sprite nameSprite;
        Role status = DUCK;
        int playerID = 0;
        bool isEliminated = false;

        Animation anim; // gives each player owns their own animation state
    };

    struct Button {
        Sprite normal;
        Sprite hover;
        SDL_Rect rect = {0,0,0,0}; // position and size in base coords
        bool isHovered = false;
    };

    struct Assets {
        Sprite duckSpriteSheet;
        Sprite itSpriteSheet;

        // goose sprite sheets
        Sprite gooseFlapSheet;
        Sprite gooseIdleSheet;
        Sprite gooseRunSheet;
        Sprite gooseWalkSheet;

        Sprite winnerBox;
        Button playAgainBtn;
        Button gameOverQuitBtn;
        Sprite tapBarUI;
        Sprite duckCallout;
        Sprite gooseCallout;
        Sprite conf_playersUI;

        //duck animations
        Animation duck_idle_normal;
        Animation duck_walk_normal;
        Animation duck_idle_bounce;
        Animation duck_walk_bounce;

        //IT animations
        Animation it_idle_normal;
        Animation it_walk_normal;
        Animation it_idle_bounce;
        Animation it_walk_bounce;
        
        //goose animations
        Animation goose_flap;
        Animation goose_idle;
        Animation goose_run;
        Animation goose_walk;
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

        //player setup

        Sprite enterNameUI;
        Sprite textBoxUI;
        Sprite playerListBox;

        Button addPlayerBtn;
        Button uploadFileBtn;
        Button startBtn;

        //main game loop
        Sprite itLabelBox;

        //game directions and duck or goose choices 
        Button dirLeftBtn;   // left direction
        Button dirRightBtn;  // right direction
        Button duckBtn;      // DUCK choice
        Button gooseBtn;     // GOOSE choice

        //round end or game over buttons
        Sprite cookingPotListUI;
        Sprite roundResultUI;

        Button continueBtn;
        Button quitRoundBtn;

        //config screen
        Button configureBtn;        // on setup screen
        Button removePlayerBtn;     // on config screen, one per player (we'll handle dynamically)
        Button backBtn;             // back to setup from config

        
    } assets;

   

    std::vector<SDL_Texture*> registeredTextures;
    //text
    Sprite inputTextSprite;
    std::string lastRenderedInput = "";
    std::string currentInput = "";
    std::string winnerName = "";

    
    static constexpr int MAX_NAME_LENGTH = 12;

    // game data
    DCList circle;
    std::vector<Player> activePlayers; // master list, parallel to circle
    std::vector<Player> cookingPot;
 
    // gameplay state
    unsigned int round = 0;
    int itIndex = 0;       // who is IT in the circle
    int tapperIndex = 0;       // IT's current position walking the circle
    bool goingClockwise = true;  // direction IT is walking
    int selectedIndex = 0;      // which player IT is hovering over
    
    int cookingPotScroll = 0; // how many entries scrolled down
    static constexpr int POT_VISIBLE = 8; // how many names fit on screen at once

    int configScroll = 0;
    static constexpr int CONFIG_VISIBLE = 8;

    /*deferred removal flag wait! they don't love you like I love you*/
    bool pendingElimination = false;
    int  eliminatedIndex = -1;

    // IT selection spin
    bool isSpinning = false;
    int  spinIndex = 0;          // currently highlighted player
    int  spinSpeed = 50;         // ms per step, starts fast
    Uint32 spinLastStep = 0;     // time of last highlight advance
    int  spinStepsLeft = 0;      // how many more steps before landing

    // circle tween
    float circleRadius = 180.0f;  // current rendered radius (was hardcoded int)
    float circleTargetRadius = 180.0f;
    float circleCY = 300.0f;  // current center Y
    float circleTargetCY = 300.0f;
    float circleScale = 1.0f;    // duck scale during tween
    float circleTargetScale = 1.0f;

    // IT walker position
    float itWalkerX = 0.0f;
    float itWalkerY = 0.0f;
    float itWalkerTargetX = 0.0f;
    float itWalkerTargetY = 0.0f;
    // tap meter
    float tapEnergy = 0.0f;   // current fill 0.0 - 1.0
    float targetPos = 0.5f;   // where the bar is (0.0 - 1.0)
    float meterDecay = 0.02f;  // how fast energy drains per frame
    float meterFill = 0.15f;  // how much each tap adds

    int   barChangeCount = 0;      // how many times bar has moved
    int   maxBarChanges = 8;      // scales with player count
    bool  chaseActive = false;  // goose is currently chasing

    // round
    int gooseIndex = -1;// who got called goose

    // state
    GameState state = GameState::SPLASH;
    bool needstoRedraw = true;
    int  menuSelection = 0;

    // SDL
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_AudioDeviceID audioDevice = 0;

    TTF_Font* font = nullptr; // 0
    TTF_Font* fontLarge = nullptr; // 1

    // base resolution — logical size, never changes
    static constexpr int baseWidth = 800;
    static constexpr int baseHeight = 600;

    //styles for blinks and fades
    Uint32 fadeTimer = 0;
    float  fadeAlpha = 255.0f;
    float  fadeSpeed = 150.0f; // alpha units per second
    bool   fadingOut = true;

    Uint32 blinkTimer = 0;
    bool blinkVisible = true;
    static constexpr Uint32 BLINK_INTERVAL = 500; // ms

    //METHODS:
   
    // SDL lifecycle and helpers
    bool initSDL();
    void initAssets();
    void getLogicalMouse(int&, int&);

    void shutdownSDL();

    // sprites, animations and buttons
    Sprite loadSprite(const char* path, float scale = 1.0f);
    Animation loadAnimation(SDL_Texture* sheet, int, int, int, Uint32, int);

    void loadPlayersFromFile(const std::string& path);
    bool openFileDialog(std::string& outPath);

    void advanceAnimation(Animation& anim);
    float lerp(float a, float b, float t);

    void updateITWalkerTarget();
    bool isHovered(const SDL_Rect& rect, int mx, int my);

    void startNextRound();

    // renderers

    Sprite renderTextSprite(const std::string& text, SDL_Color color, int);
    void renderAnimation(const Animation& anim, int x, int y, float scale);

    void renderButton(const Button& btn);
    void renderPlayer(Player& player, int x, int y, bool highlighted);

    void renderSplash();
    void renderMM();
    void renderOptions();
    void renderSetup();
    void renderConfigure();
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
    void handleConfigureEvents(SDL_Event& e, bool& running);
    void handleGameEvents(SDL_Event& e, bool& running);
    void handlePauseEvents(SDL_Event& e, bool& running);
    void handleRoundEndEvents(SDL_Event& e, bool& running);
    void handleGameOverEvents(SDL_Event& e, bool& running);

    // game logic
    void updatePos();

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