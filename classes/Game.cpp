#include "Game.h"
#include <iostream>

/*
need to create a structure that has the menu options, keyboard presses, and then the sprites
*/
Game::Game() : round(0), itIndex(0) {
    if (!initSDL()) {
        std::cerr << "SDL failed to initialize: " << SDL_GetError() << "\n";
    }
}

Game::~Game() {
    shutdownSDL();
}
bool Game::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow(
        "Duck Duck Goose",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        baseWidth, baseHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        return false;
    }

    SDL_RenderSetLogicalSize(renderer, baseWidth, baseHeight);
    SDL_RenderSetIntegerScale(renderer, SDL_FALSE);

    IMG_Init(IMG_INIT_PNG);
    initAssets();
    return true;
}

void Game::initAssets()
{
    assets.tapperSprite = loadSprite("assets/sprites/tapper.png", 1.0f);
    assets.gooseSprite = loadSprite("assets/sprites/goose.png", 1.0f);
    assets.itSprite = loadSprite("assets/sprites/it.png", 1.0f);
    assets.bgTile = loadSprite("assets/ui/backgroundpattern.png", 1.0f);
    //splash screen
    assets.splashTitle = loadSprite("assets/ui/duckduckmaintitle.png", 1.0f);
    assets.start = loadSprite("assets/ui/press_to_start.png", 1.0f);
    //main menu
    assets.mainMenuTitle = loadSprite("assets/ui/ddg_mainmenutitle.png", 1.0f);

    assets.playButton.normal = loadSprite("assets/ui/play_button.png", 1.0f);
    assets.playButton.hover = loadSprite("assets/ui/play_button_hover.png", 1.0f);
    assets.playButton.rect = {222, 198, 372, 102};

    assets.optionsButton.normal = loadSprite("assets/ui/options_button.png", 1.0f);
    assets.optionsButton.hover = loadSprite("assets/ui/options_button_hover.png", 1.0f);
    assets.optionsButton.rect = {17, 337, 362, 162};

    assets.quitButton.normal = loadSprite("assets/ui/quit_button.png", 1.0f);
    assets.quitButton.hover = loadSprite("assets/ui/quit_button_hover.png", 1.0f);
    assets.quitButton.rect = {428, 385, 372, 126};


}

void Game::getLogicalMouse(int& mx, int& my) {
    int rawX, rawY;
    SDL_GetMouseState(&rawX, &rawY);
    float lx, ly;
    SDL_RenderWindowToLogical(renderer, rawX, rawY, &lx, &ly);
    mx = static_cast<int>(lx);
    my = static_cast<int>(ly);
}
void Game::shutdownSDL() {
//go through each texture in the register and DESTROY! them
    for (SDL_Texture * tex : registeredTextures)
        if (tex) SDL_DestroyTexture(tex);

    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

Game::Sprite Game::loadSprite(const char* path, float scale)
{
    Sprite s;
    s.texture = IMG_LoadTexture(renderer, path);

    if (!s.texture) {
        std::cerr << "Failed to load sprite" << path << ": " << IMG_GetError() << "\n";
        return s;
    }

    SDL_QueryTexture(s.texture, nullptr, nullptr, &s.src_width, &s.src_height);
    s.scaleFactor = scale;
    registeredTextures.push_back(s.texture); //register each texture so they can be destroyed later.
    return s;

}

void Game::renderButton(const Button& btn) {
    SDL_Texture* tex = btn.isHovered ? btn.hover.texture : btn.normal.texture;
    SDL_RenderCopy(renderer, tex, nullptr, &btn.rect);
}

bool Game::isHovered(const SDL_Rect& rect, int mx, int my) {//mouse hover checks
    return mx >= rect.x && mx <= rect.x + rect.w &&
        my >= rect.y && my <= rect.y + rect.h;
}

void Game::renderSplash() {
    SDL_Rect dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &dst);

    // title centered
    SDL_Rect titleDst = {
        baseWidth / 2 - 757 / 2,   // logical width / 2 to center
        100,
        757,                         // logical size, not src size
        244
    };
    SDL_RenderCopy(renderer, assets.splashTitle.texture, nullptr, &titleDst);

    SDL_SetTextureAlphaMod(assets.start.texture, static_cast<Uint8>(fadeAlpha));
    SDL_Rect promptDst = {
        306,
        382,
        207,
        39
    };
    SDL_RenderCopy(renderer, assets.start.texture, nullptr, &promptDst);
    SDL_SetTextureAlphaMod(assets.start.texture, 255); // reset after
}

void Game::renderMM()
{
    SDL_Rect bg_dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg_dst);

    SDL_Rect titleDst = {
        52,   
        33,
        716,                         
        128
    };
    SDL_RenderCopy(renderer, assets.mainMenuTitle.texture, nullptr, &titleDst);

    renderButton(assets.playButton);
    renderButton(assets.optionsButton);
    renderButton(assets.quitButton);

}

void Game::renderOptions()
{

}

void Game::renderSetup()
{
    SDL_Rect bg_dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg_dst);

    //you are actively working on this screen
}

void Game::renderGame() {
    // background
    for (int y = 0; y < baseHeight; y += 100)
        for (int x = 0; x < baseWidth; x += 100) {
            SDL_Rect dst = { x, y, 100, 100 };
            SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &dst);
        }

    // draw circle of players
    int   cx = baseWidth / 2;
    int   cy = baseHeight / 2;
    int   radius = 180;
    int   count = circle.getSize();
    float step = (2.0f * M_PI) / count; //angle diff between each player

    for (int i = 0; i < count; i++) {
        int x = cx + static_cast<int>(radius * cos(step * i)) - 32; //anchor point diff
        int y = cy + static_cast<int>(radius * sin(step * i)) - 32;
        renderPlayer(activePlayers[i], x, y);
    }

    if (playPhase == PlayPhase::CHASING)
        renderTapMeter();
}

void Game::renderTapMeter()
{
    //TODO: finish this
}

void Game::renderPause()
{

}

void Game::renderRoundEnd()
{

}

void Game::renderGameOver()
{

}


//Event Handling
void Game::handleEvents(SDL_Event& e, bool& running) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;

        // splash — any input advances to menu
        if (state == GameState::SPLASH) {
            if (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN) {
                state = GameState::MAIN_MENU;
                needstoRedraw = true;
            }
            return;
        }

        if (state == GameState::MAIN_MENU) {
            if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN) {
                int mx, my;
                getLogicalMouse(mx, my);

                assets.playButton.isHovered = isHovered(assets.playButton.rect, mx, my);
                assets.optionsButton.isHovered = isHovered(assets.optionsButton.rect, mx, my);
                assets.quitButton.isHovered = isHovered(assets.quitButton.rect, mx, my);

                // sync menuSelection to whichever is hovered
                if (assets.playButton.isHovered)    menuSelection = 0;
                if (assets.optionsButton.isHovered) menuSelection = 1;
                if (assets.quitButton.isHovered)    menuSelection = 2;

                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (menuSelection == 0) state = GameState::PLAYER_SETUP;
                    if (menuSelection == 1) state = GameState::OPTIONS;
                    if (menuSelection == 2) running = false;
                }
                needstoRedraw = true;
            }
        }
        if (e.type == SDL_KEYDOWN) {
            switch (state) {
            case GameState::MAIN_MENU:
                if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_LEFT)
                    menuSelection = (menuSelection - 1 + 3) % 3;
                if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_RIGHT)
                    menuSelection = (menuSelection + 1) % 3;

                // sync hover state to keyboard selection
                assets.playButton.isHovered = (menuSelection == 0);
                assets.optionsButton.isHovered = (menuSelection == 1);
                assets.quitButton.isHovered = (menuSelection == 2);

                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (menuSelection == 0) state = GameState::PLAYER_SETUP;
                    if (menuSelection == 1) state = GameState::OPTIONS;
                    if (menuSelection == 2) running = false;
                }
                needstoRedraw = true;
                break;
            case GameState::PLAYING:
                handleGameEvents(e, running);
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    state = GameState::PAUSED;
                needstoRedraw = true;
                break;

            case GameState::PAUSED:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    state = GameState::PLAYING;
                needstoRedraw = true;
                break;

            default:
                if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                break;
            }
        }
    }
}
//STUBBED
void Game::handleMenuEvents(SDL_Event& e, bool& running)
{
    
}

//STUBBED
void Game::handleOptionsEvents(SDL_Event& e, bool& running)
{}

//STUBBED
void Game::handleSetupEvents(SDL_Event & e, bool& running)
{}

void Game::handleGameEvents(SDL_Event& e, bool& running) {
    if (e.type != SDL_KEYDOWN) return;

    switch (playPhase) {

    case PlayPhase::CHOOSING_DIRECTION:
        if (e.key.keysym.sym == SDLK_LEFT) {
            goingClockwise = false;
            playPhase = PlayPhase::WALKING;
        }
        if (e.key.keysym.sym == SDLK_RIGHT) {
            goingClockwise = true;
            playPhase = PlayPhase::WALKING;
        }
        break;

    case PlayPhase::WALKING:
        // cycle through circle
        if (e.key.keysym.sym == SDLK_LEFT)
            selectedIndex = goingClockwise
            ? (selectedIndex - 1 + circle.getSize()) % circle.getSize()
            : (selectedIndex + 1) % circle.getSize();

        if (e.key.keysym.sym == SDLK_RIGHT)
            selectedIndex = goingClockwise
            ? (selectedIndex + 1) % circle.getSize()
            : (selectedIndex - 1 + circle.getSize()) % circle.getSize();

        // duck — move on
        if (e.key.keysym.sym == SDLK_LEFT && selectedIndex != itIndex) {
            // just highlight as duck, keep walking
        }

        // goose — start chase
        if (e.key.keysym.sym == SDLK_RIGHT) {
            gooseIndex = selectedIndex;
            setRole(cookingPot[gooseIndex], GOOSE);

            // init tap meter
            tapEnergy = 0.0f;
            barChangeCount = 0;
            maxBarChanges = std::min(8, circle.getSize() * 2);
            targetPos = 0.25f + static_cast<float>(rand()) /
                (static_cast<float>(RAND_MAX / 0.65f));
            chaseActive = true;
            playPhase = PlayPhase::CHASING;
        }
        break;
    /* NEEDS FORMAL DOCUMENTATION */
    case PlayPhase::CHASING:
        // spacebar or up to tap
        if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) {
            tapEnergy += meterFill;
            if (tapEnergy > 1.0f) tapEnergy = 1.0f;

            // check if hit the target zone
            float zoneSize = 0.1f;
            float zoneBottom = targetPos - zoneSize / 2;
            float zoneTop = targetPos + zoneSize / 2;

            if (tapEnergy >= zoneBottom && tapEnergy <= zoneTop) {
                barChangeCount++;

                // move target to new random position
                targetPos = 0.1f + static_cast<float>(rand()) /
                    (static_cast<float>(RAND_MAX / 0.8f));

                // check win condition
                float successThreshold = maxBarChanges * 0.7f;
                if (barChangeCount >= successThreshold) {
                    // IT escaped — goose becomes IT, IT takes goose's spot
                    int oldIt = itIndex;
                    itIndex = gooseIndex;
                    selectedIndex = oldIt;
                    setRole(cookingPot[itIndex], IT);
                    setRole(cookingPot[selectedIndex], DUCK);
                    chaseActive = false;
                    playPhase = PlayPhase::CHOOSING_DIRECTION;

                    // check if round is over
                    if (circle.getSize() <= 1) state = GameState::GAME_OVER;
                    else                       state = GameState::ROUND_END;
                }

                // check if bar changes exhausted
                if (barChangeCount >= maxBarChanges) {
                    // IT failed — gets tagged, goes to cooking pot
                    addToPot(cookingPot[itIndex]);
                    circle.remove(itIndex);
                    itIndex = gooseIndex; // goose becomes next IT
                    chaseActive = false;
                    playPhase = PlayPhase::CHOOSING_DIRECTION;

                    if (circle.getSize() <= 1) state = GameState::GAME_OVER;
                    else                       state = GameState::ROUND_END;
                }
            }
        }
        break;
    }
    needstoRedraw = true;
}

//STUBBED
void Game::handlePauseEvents(SDL_Event & e, bool& running)
{}

//STUBBED
void Game::updatePos()
{
    if (playPhase != PlayPhase::CHASING) return;

    tapEnergy -= meterDecay;
    if (tapEnergy < 0.0f) tapEnergy = 0.0f;

    // decay gets faster as bar changes increase
    float urgency = static_cast<float>(barChangeCount) / maxBarChanges;
    meterDecay = 0.02f + (urgency * 0.03f); // ramps from 0.02 to 0.05

    needstoRedraw = true;
}

//STUBBED
void Game::renderPlayer(const Player & player, int x, int y)
{}

//Game Logic

Game::Player Game::createPlayer(std::string name) {
    Player p;
    p.name = name;
    p.status = DUCK;
    p.playerID = static_cast<int>(circle.getSize()); // placeholder ID logic
    circle.insert(p.name, p.playerID);
    activePlayers.push_back(p); 

    return p;
}

const Game::Player& Game::getPlayer(int id) const {
    // assumes cookingPot is indexed by playerID
    return cookingPot.at(id);
}

void Game::setRole(Player& player, Role role) {
    player.status = role;
}

void Game::addToPot(const Player& player) {
    cookingPot.push_back(player);
}

void Game::createCircle() {
    int count = circle.getSize();
    itIndex = rand() % count;
    selectedIndex = itIndex;
    setRole(activePlayers[itIndex], IT);
    playPhase = PlayPhase::CHOOSING_DIRECTION;
    state = GameState::PLAYING;
}
//Main Game Loop

void Game::play() {
    bool running = true;
    SDL_Event e;
    Uint32 lastUpdate = SDL_GetTicks();

    while (running) {
        //events
        handleEvents(e, running);

        //update logic on a timer
        Uint32 now = SDL_GetTicks();
        if (now - lastUpdate >= 500) {
            if (state == GameState::PLAYING) updatePos();

            lastUpdate = now;
            needstoRedraw = true;
        }

        float deltaTime = (now - fadeTimer) / 1000.0f; // seconds since last frame
        fadeTimer = now;

        if (state == GameState::SPLASH) {
            if (fadingOut) {
                fadeAlpha -= fadeSpeed * deltaTime;
                if (fadeAlpha <= 50.0f) fadingOut = false;
            }
            else {
                fadeAlpha += fadeSpeed * deltaTime;
                if (fadeAlpha >= 255.0f) fadingOut = true;
            }
            needstoRedraw = true;
        }

        //render only when something changed
        if (needstoRedraw) {
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderClear(renderer);
            //state machine!!!
            switch (state) {
                case GameState::SPLASH:
                    renderSplash();
                    break;
                case GameState::MAIN_MENU:
                     renderMM();       
                     break;
                case GameState::OPTIONS:      
                    renderOptions();  
                    break;
                case GameState::PLAYER_SETUP: 
                    renderSetup();    
                    break;
                case GameState::PLAYING:
                    renderGame();     
                    break;
                case GameState::PAUSED:       
                    renderPause();    
                    break;
                case GameState::ROUND_END:    
                    renderRoundEnd(); 
                    break;
                case GameState::GAME_OVER:    
                    renderGameOver(); 
                    break;
            }

            SDL_RenderPresent(renderer);
            needstoRedraw = false;
        }

        SDL_Delay(16);
    }
}