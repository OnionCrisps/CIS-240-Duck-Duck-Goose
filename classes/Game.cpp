#include "Game.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <commdlg.h>

/*Gavin Harban, Jon Moore, Game.h CIS 240-1 Data Structures Project*/

#pragma comment(lib, "comdlg32.lib")
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

    TTF_Init();
    font = TTF_OpenFont("assets/font/BlackHanSans-Regular.ttf", 24);
    fontLarge = TTF_OpenFont("assets/font/BlackHanSans-Regular.ttf", 96);

    if (!font || !fontLarge) std::cerr << "Font failed: " << TTF_GetError() << "\n";

    IMG_Init(IMG_INIT_PNG);
    initAssets();
    return true;
}

void Game::initAssets()
{
    //sprite sheets
    assets.duckSpriteSheet = loadSprite("assets/sprites/duck.png", 1.0f);
    assets.itSpriteSheet = loadSprite("assets/sprites/it.png", 1.0f);

    //add sprite sheets for goose:
    assets.gooseFlapSheet = loadSprite("assets/sprites/Flap.png", 1.0f);
    assets.gooseIdleSheet = loadSprite("assets/sprites/Idle.png", 1.0f);
    assets.gooseRunSheet = loadSprite("assets/sprites/Run.png", 1.0f);
    assets.gooseWalkSheet = loadSprite("assets/sprites/Walk.png", 1.0f);

    //Animations for Duck:
    assets.duck_idle_normal = loadAnimation(assets.duckSpriteSheet.texture, 32, 32, 2, 200, 0);
    assets.duck_walk_normal = loadAnimation(assets.duckSpriteSheet.texture, 32, 32, 6, 100, 1);
    assets.duck_idle_bounce = loadAnimation(assets.duckSpriteSheet.texture, 32, 32, 4, 150, 2);
    assets.duck_walk_bounce = loadAnimation(assets.duckSpriteSheet.texture, 32, 32, 6, 60, 3);

    //Animations for IT:
    assets.it_idle_normal = loadAnimation(assets.itSpriteSheet.texture, 32, 32, 2, 200, 0);
    assets.it_walk_normal = loadAnimation(assets.itSpriteSheet.texture, 32, 32, 6, 100, 1);
    assets.it_idle_bounce = loadAnimation(assets.itSpriteSheet.texture, 32, 32, 4, 150, 2);
    assets.it_walk_bounce = loadAnimation(assets.itSpriteSheet.texture, 32, 32, 6, 60, 3);

    //Animations for Goose:
    assets.goose_flap = loadAnimation(assets.gooseFlapSheet.texture, 64, 64, 4, 150, 0);
    assets.goose_idle = loadAnimation(assets.gooseIdleSheet.texture, 64, 64, 2, 200, 0);
    assets.goose_run = loadAnimation(assets.gooseRunSheet.texture, 64, 64, 4, 80, 0);
    assets.goose_walk = loadAnimation(assets.gooseWalkSheet.texture, 64, 64, 4, 120, 0);

    assets.bgTile = loadSprite("assets/ui/backgroundpattern.png", 1.0f);

    //splash screen
    assets.splashTitle = loadSprite("assets/ui/duckduckmaintitle.png", 1.0f);
    assets.start = loadSprite("assets/ui/press_to_start.png", 1.0f);

    //main menu
    assets.mainMenuTitle = loadSprite("assets/ui/ddg_mainmenutitle.png", 1.0f);

    assets.playButton.normal = loadSprite("assets/ui/play_button.png", 1.0f);
    assets.playButton.hover = loadSprite("assets/ui/play_button_hover.png", 1.0f);
    assets.playButton.rect = {222, 198, 372, 102};

    assets.quitButton.normal = loadSprite("assets/ui/quit_button.png", 1.0f);
    assets.quitButton.hover = loadSprite("assets/ui/quit_button_hover.png", 1.0f);
    assets.quitButton.rect = {228, 385, 372, 126};

    //setup screen

    assets.enterNameUI = loadSprite("assets/ui/ENTER_NAME_UI.png", 1.0f);
    assets.textBoxUI = loadSprite("assets/ui/TEXT_BOX_UI.png", 1.0f);
    assets.playerListBox = loadSprite("assets/ui/UI_BASIC_REC.png", 1.0f);
    assets.addPlayerBtn.normal = loadSprite("assets/ui/add_player_button.png",1.0f);
    assets.addPlayerBtn.hover = loadSprite("assets/ui/add_player_button_hover.png", 1.0f);
    assets.addPlayerBtn.rect = {8, 241, 371, 67};

    assets.uploadFileBtn.normal = loadSprite("assets/ui/upload_file_button.png", 1.0f);
    assets.uploadFileBtn.hover = loadSprite("assets/ui/upload_file_button_hover.png", 1.0f);
    assets.uploadFileBtn.rect = { 16, 348, 355, 67 };

    assets.startBtn.normal = loadSprite("assets/ui/start_button.png", 1.0f);
    assets.startBtn.hover = loadSprite("assets/ui/start_button_hover.png", 1.0f);
    assets.startBtn.rect = { 423, 423, 375, 154 };
    //main game loop

    assets.itLabelBox = loadSprite("assets/ui/basic_label_box.png", 1.0f);

    assets.dirLeftBtn.normal = loadSprite("assets/ui/dir_left.png", 1.0f);
    assets.dirLeftBtn.hover = loadSprite("assets/ui/dir_left_hover.png", 1.0f);
    assets.dirLeftBtn.rect = { 145, 379, 165, 165 };

    assets.dirRightBtn.normal = loadSprite("assets/ui/dir_right.png", 1.0f);
    assets.dirRightBtn.hover = loadSprite("assets/ui/dir_right_hover.png", 1.0f);
    assets.dirRightBtn.rect = { 499, 384, 164, 165 };

    assets.duckBtn.normal = loadSprite("assets/ui/duck_button.png", 1.0f);
    assets.duckBtn.hover = loadSprite("assets/ui/duck_button_hover.png", 1.0f);
    assets.duckBtn.rect = {0, 363, 429, 220};

    assets.gooseBtn.normal = loadSprite("assets/ui/goose_button.png", 1.0f);
    assets.gooseBtn.hover = loadSprite("assets/ui/goose_button_hover.png", 1.0f);
    assets.gooseBtn.rect = { 404, 412, 405, 142};

    //TODO: MATCH TO CORRECT ASSETS AND SIZES
    assets.cookingPotListUI = loadSprite("assets/ui/COOKING_POT_LIST_UI.png", 1.0f);
    assets.roundResultUI = loadSprite("assets/ui/round_result_ui.png", 1.0f);

    assets.continueBtn.normal = loadSprite("assets/ui/continue_button.png", 1.0f);
    assets.continueBtn.hover = loadSprite("assets/ui/continue_button_hover.png", 1.0f);
    assets.continueBtn.rect = { 518, 506, 320, 54 };  // bottom right

    assets.quitRoundBtn.normal = loadSprite("assets/ui/quit_button.png", 1.0f);
    assets.quitRoundBtn.hover = loadSprite("assets/ui/quit_button_hover.png", 1.0f);
    assets.quitRoundBtn.rect = { 18, 492, 213, 73 };  // bottom left

    //TODO: MATCH TO CORRECT ASSETS AND SIZES
    assets.configureBtn.normal = loadSprite("assets/ui/configure_button.png", 1.0f);
    assets.configureBtn.hover = loadSprite("assets/ui/configure_button_hover.png", 1.0f);
    assets.configureBtn.rect = { 16, 513, 355, 67 }; 

    assets.backBtn.normal = loadSprite("assets/ui/menu_button.png", 1.0f);
    assets.backBtn.hover = loadSprite("assets/ui/menu_button_hover.png", 1.0f);
    assets.backBtn.rect = { 19, 498, 279, 76 }; 

    // game over screen
    assets.winnerBox = loadSprite("assets/ui/winner_box.png", 1.0f);
    assets.playAgainBtn.normal = loadSprite("assets/ui/play_again_button.png", 1.0f);
    assets.playAgainBtn.hover = loadSprite("assets/ui/play_again_button_hover.png", 1.0f);
    assets.playAgainBtn.rect = { 20, 480, 260, 80 };   // bottom left

    assets.gameOverQuitBtn.normal = loadSprite("assets/ui/quit_button.png", 1.0f);
    assets.gameOverQuitBtn.hover = loadSprite("assets/ui/quit_button_hover.png", 1.0f);
    assets.gameOverQuitBtn.rect = { 490, 480, 220, 80 }; // bottom right

    // tap bar
    assets.tapBarUI = loadSprite("assets/ui/TAP_BAR_UI.png", 1.0f);

    // callouts
    assets.duckCallout = loadSprite("assets/ui/duck_ui_callout.png", 1.0f);
    assets.gooseCallout = loadSprite("assets/ui/goose_ui_callout.png", 1.0f);

    assets.conf_playersUI = loadSprite("assets/ui/config_players_box.png", 1.0f);

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

    TTF_CloseFont(font);
    TTF_CloseFont(fontLarge);
    TTF_Quit();
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

Game::Animation Game::loadAnimation(SDL_Texture* sheet, int frameW, int frameH, int frameCount, Uint32 frameDelay, int row)
{
    Animation a;
    a.sheet = sheet;
    a.frameW = frameW;
    a.frameH = frameH;
    a.frameCount = frameCount;
    a.frameDelay = frameDelay;
    a.row = row; 
    return a;
}

void Game::loadPlayersFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << "\n";
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        // trim carriage return and trailing spaces
        while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
            line.pop_back();
        if (line.empty()) continue;
        if (line.length() > MAX_NAME_LENGTH)
            line = line.substr(0, MAX_NAME_LENGTH);
        if (circle.getSize() >= 20) break;
        createPlayer(line);
    }
}

bool Game::openFileDialog(std::string& outPath) {
#ifdef _WIN32
    char filename[MAX_PATH] = "";
    OPENFILENAMEA ofn = {};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = "Select Player List";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameA(&ofn)) {
        outPath = filename;
        return true;
    }
#endif
    return false;
}

void Game::advanceAnimation(Animation& anim)
{
    Uint32 now = SDL_GetTicks();
    if (now - anim.lastFrame >= anim.frameDelay) {
        anim.currentFrame = (anim.currentFrame + 1) % anim.frameCount;
        anim.lastFrame = now;
        needstoRedraw = true;
    }
}

float Game::lerp(float a, float b, float t)
{
    return a + (b-a) * t;
}

void Game::updateITWalkerTarget() {
    int cx = (baseWidth / 2)-10;
    int cy = static_cast<int>(circleCY);
    double step = (2.0 * M_PI) / circle.getSize();
    double outerRadius = circleRadius + 40.0; // just outside the circle

    itWalkerTargetX = cx + outerRadius * cos(step * selectedIndex) - 16;
    itWalkerTargetY = cy + outerRadius * sin(step * selectedIndex) - 16;
}

void Game::updateGooseWalkerTarget() {
    int cx = (baseWidth / 2) - 10;
    int cy = static_cast<int>(circleCY);
    double step = (2.0 * M_PI) / circle.getSize();
    double outerRadius = circleRadius + 40.0;

    gooseWalkerTargetX = cx + outerRadius * cos(step * gooseWalkerIndex) - 16;
    gooseWalkerTargetY = cy + outerRadius * sin(step * gooseWalkerIndex) - 16;
}

bool Game::isHovered(const SDL_Rect& rect, int mx, int my) {//mouse hover checks
    return mx >= rect.x && mx <= rect.x + rect.w &&
        my >= rect.y && my <= rect.y + rect.h;
}

void Game::startNextRound() {
    if (activePlayers.empty() || circle.getSize() == 0) {
        state = GameState::SPLASH;
        return;
    }
    if (itIndex >= (int)activePlayers.size()) itIndex = 0;

    gooseLastStep = 0;
    itChaseLastStep = 0;
    chaseStartTime = 0;
    chaseActive = false;
    tapEnergy = 0.0f;

    for (Player& p : activePlayers) {
        p.anim = Animation();
        p.status = DUCK;
    }

    if (pendingElimination && eliminatedIndex >= 0) {
        activePlayers.erase(activePlayers.begin() + eliminatedIndex);
        circle.remove(eliminatedIndex);
        if (itIndex > eliminatedIndex) itIndex--;
        pendingElimination = false;
        eliminatedIndex = -1;
    }

    if (activePlayers.empty()) { state = GameState::SPLASH; return; }
    if (itIndex >= (int)activePlayers.size()) itIndex = 0;

    pendingItIndex = itIndex;
    itIndex = 0;

    circleTargetRadius = 180.0f;
    circleTargetCY = 300.0f;
    circleTargetScale = 1.0f;
    cookingPotScroll = 0;
    spinIndex = 0;
    spinSpeed = 50;
    spinStepsLeft = 20 + (circle.getSize() * 3) + pendingItIndex;
    isSpinning = true;
    playPhase = PlayPhase::SPINNING;
    state = GameState::PLAYING;
}

void Game::resetGame() {
    // clear all players and data
    activePlayers.clear();
    cookingPot.clear();
    circle.~DCList();
    new (&circle) DCList();

    // reset all game state
    winnerName = "";
    cookingPotScroll = 0;
    configScroll = 0;
    itIndex = 0;
    gooseIndex = -1;
    selectedIndex = 0;
    pendingElimination = false;
    eliminatedIndex = -1;
    chaseActive = false;
    barChangeCount = 0;
    tapEnergy = 0.0f;
    meterDecay = 0.002f;
    activeCallout = CalloutType::NONE;
    goingClockwise = true;
    chaseStartTime = 0;

    // reset tween
    circleRadius = 180.0f; circleTargetRadius = 180.0f;
    circleCY = 300.0f; circleTargetCY = 300.0f;
    circleScale = 1.0f;   circleTargetScale = 1.0f;
    itWalkerX = 0.0f;   itWalkerTargetX = 0.0f;
    itWalkerY = 0.0f;   itWalkerTargetY = 0.0f;
    gooseWalkerX = 0.0f;   gooseWalkerTargetX = 0.0f;
    gooseWalkerY = 0.0f;   gooseWalkerTargetY = 0.0f;

    playPhase = PlayPhase::CHOOSING_DIRECTION;
    state = GameState::PLAYER_SETUP;
    SDL_StartTextInput();
}
Game::Sprite Game::renderTextSprite(const std::string& text, SDL_Color color, int size) {
    Sprite s;
    SDL_Surface* surf = (size == 0) ? TTF_RenderText_Blended(font, text.c_str(), color) : TTF_RenderText_Blended(fontLarge, text.c_str(), color);

    if (!surf) return s;

    s.texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_QueryTexture(s.texture, nullptr, nullptr, &s.src_width, &s.src_height);

    s.scaleFactor = 1.0f;

    SDL_FreeSurface(surf);
    return s;
}

void Game::renderAnimation(const Animation& anim, int x, int y, float scale)
{
    SDL_Rect src = {
            anim.currentFrame * anim.frameW,  // x offset into sheet
            anim.row * anim.frameH,  // y offset for this row
            anim.frameW,
            anim.frameH
    };
    SDL_Rect dst = {
        x, y,
        static_cast<int>(anim.frameW * scale),
        static_cast<int>(anim.frameH * scale)
    };
    SDL_RenderCopy(renderer, anim.sheet, &src, &dst);
}

void Game::renderButton(const Button& btn) {
    SDL_Texture* tex = btn.isHovered ? btn.hover.texture : btn.normal.texture;
    SDL_RenderCopy(renderer, tex, nullptr, &btn.rect);
}

void Game::renderPlayer(Player& player, int x, int y, bool highlighted) {
    if (highlighted) {
        float scale = circleScale * 3.0f;
        int spriteW = static_cast<int>(player.anim.frameW * scale);

        // triangle pointing down above the duck's head
        int tipX = x + spriteW / 2;       // center of sprite
        int tipY = y - 4;                  // just above head
        int baseY = tipY - 12;             // triangle height
        int baseHalf = 8;                   // half width of triangle base

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // draw filled triangle with horizontal lines
        for (int row = 0; row <= 12; row++) {
            float t = (float)row / 12.0f;
            int halfW = static_cast<int>(baseHalf * (1.0f - t));
            SDL_RenderDrawLine(renderer,
                tipX - halfW, baseY + row,
                tipX + halfW, baseY + row);
        }
    }

    if (player.status == GOOSE) {
        float renderScale = circleScale * 1.5f;
        Animation* gooseSrc = (playPhase == PlayPhase::CHASING)
            ? &assets.goose_run
            : &assets.goose_walk;

        if (player.anim.sheet != gooseSrc->sheet || player.anim.row != gooseSrc->row) {
            player.anim.sheet = gooseSrc->sheet;
            player.anim.frameW = gooseSrc->frameW;
            player.anim.frameH = gooseSrc->frameH;
            player.anim.frameCount = gooseSrc->frameCount;
            player.anim.frameDelay = gooseSrc->frameDelay;
            player.anim.row = gooseSrc->row;
            player.anim.currentFrame = 0;
            player.anim.lastFrame = 0;
        }
        advanceAnimation(player.anim);
        renderAnimation(player.anim, x, y, renderScale);

        int gSpriteW = static_cast<int>(player.anim.frameW * renderScale);
        int gSpriteH = static_cast<int>(player.anim.frameH * renderScale);
        SDL_Rect nameDst = {
            x + gSpriteW / 2 - player.nameSprite.src_width / 2,
            y + gSpriteH + 4,
            player.nameSprite.src_width,
            player.nameSprite.src_height
        };
        SDL_RenderCopy(renderer, player.nameSprite.texture, nullptr, &nameDst);
        return;
    }

    Animation* src = nullptr;

    if (state == GameState::PLAYER_SETUP) {
        src = &assets.duck_idle_normal;
    }
    else if (player.status == IT) {
        src = &assets.it_walk_bounce; // IT always walking
    }
    else if (player.status == DUCK && playPhase == PlayPhase::WALKING) {
        src = &assets.duck_idle_bounce;
    }
    else {
        src = &assets.duck_idle_normal;
    }

    // only resync if the animation actually changed
    if (player.anim.sheet != src->sheet || player.anim.row != src->row) {
        player.anim.sheet = src->sheet;
        player.anim.frameW = src->frameW;
        player.anim.frameH = src->frameH;
        player.anim.frameCount = src->frameCount;
        player.anim.frameDelay = src->frameDelay;
        player.anim.row = src->row;
        player.anim.currentFrame = 0;  // reset to start of new animation
        player.anim.lastFrame = 0;
    }

    advanceAnimation(player.anim);
    renderAnimation(player.anim, x, y, circleScale*3.0f);

    float scale = circleScale * 3.0f;
    int spriteW = static_cast<int>(player.anim.frameW * scale);
    int spriteH = static_cast<int>(player.anim.frameH * scale);

    SDL_Rect nameDst = {
        x + spriteW / 2 - player.nameSprite.src_width / 2,  // centered under sprite
        y + spriteH + 4,                                      // just below sprite
        player.nameSprite.src_width,
        player.nameSprite.src_height
    };
    SDL_RenderCopy(renderer, player.nameSprite.texture, nullptr, &nameDst);
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

void Game::renderMM() {
    SDL_Rect bg_dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg_dst);

    SDL_Rect titleDst = { 52, 33, 716, 128 };
    SDL_RenderCopy(renderer, assets.mainMenuTitle.texture, nullptr, &titleDst);

    renderButton(assets.playButton);
    renderButton(assets.quitButton);
}
void Game::renderSetup() {
    SDL_Rect bg_dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg_dst);

    // title / header
    SDL_Rect headerDst = {
        440,
        14,
        358,
        64
    };
    SDL_RenderCopy(renderer, assets.mainMenuTitle.texture, nullptr, &headerDst);

    // input box PNG
    SDL_Rect inputDst = {
        318,
        125,
        455,
        49
    };
    SDL_RenderCopy(renderer, assets.textBoxUI.texture, nullptr, &inputDst);

    SDL_Rect labelDst = {
        11, 117, 307, 57
    };
    SDL_RenderCopy(renderer, assets.enterNameUI.texture, nullptr, &labelDst);

    // typed text inside input box
    if (!currentInput.empty()) {
        if (currentInput != lastRenderedInput) {
            if (inputTextSprite.texture) SDL_DestroyTexture(inputTextSprite.texture);

            SDL_Color black = { 0, 0, 0, 255 };
            inputTextSprite = renderTextSprite(currentInput, black, 0);
            lastRenderedInput = currentInput;
        }
        SDL_Rect textDst = {
            404, 137, inputTextSprite.src_width, inputTextSprite.src_height
        };
        SDL_RenderCopy(renderer, inputTextSprite.texture, nullptr, &textDst);
    }

    // blinking cursor — shows after typed text
    if (blinkVisible) {
        int cursorX = 404 + (inputTextSprite.texture ? inputTextSprite.src_width : 0) + 4;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect cursor = { cursorX, 130, 2, 32 };
        SDL_RenderFillRect(renderer, &cursor);
    }

    // add player button
    renderButton(assets.addPlayerBtn);
    //upload file button
    renderButton(assets.uploadFileBtn);
    //configure buttton
    renderButton(assets.configureBtn);
    // player count box
    SDL_Rect boxDst = { 481, 223, 200, 200 };
    SDL_RenderCopy(renderer, assets.playerListBox.texture, nullptr, &boxDst);

    // player count number
    Sprite countSprite = renderTextSprite(std::to_string(circle.getSize()), { 0, 0, 0, 255 }, 1);
    SDL_Rect countDst = {
        481 + (200 / 2) - countSprite.src_width / 2,  // centered in box
        223 + (200 / 2) - countSprite.src_height / 2,
        countSprite.src_width,
        countSprite.src_height
    };
    SDL_RenderCopy(renderer, countSprite.texture, nullptr, &countDst);
    SDL_DestroyTexture(countSprite.texture);

    // start game button — only if 5+ players
    if (circle.getSize() >= 5) {
        renderButton(assets.startBtn);
    }
}

void Game::renderConfigure() {
    SDL_Rect bg = { 0, 0, baseWidth, baseHeight };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg);

    SDL_Rect confDST = { 3, 63, 768, 404 };
    SDL_RenderCopy(renderer, assets.conf_playersUI.texture, nullptr, &confDST);

    // player list — starts at 81,153 inside the box
    int listX = 81;
    int listY = 153;
    int entryH = 38;
    int visibleEnd = min(configScroll + CONFIG_VISIBLE, (int)activePlayers.size());

    for (int i = configScroll; i < visibleEnd; i++) {
        // player name
        Sprite name = renderTextSprite(
            std::to_string(i + 1) + ".  " + activePlayers[i].name, { 0,0,0,255 }, 0);
        SDL_Rect nameDst = { listX, listY, name.src_width, name.src_height };
        SDL_RenderCopy(renderer, name.texture, nullptr, &nameDst);
        SDL_DestroyTexture(name.texture);

        // remove button — red X on the right inside the box
        SDL_Rect removeRect = {444, listY, 50, 28 };
        SDL_SetRenderDrawColor(renderer, 180, 40, 40, 255);
        SDL_RenderFillRect(renderer, &removeRect);
        Sprite x = renderTextSprite("X", { 255,255,255,255 }, 0);
        SDL_Rect xDst = {
            removeRect.x + removeRect.w / 2 - x.src_width / 2,
            removeRect.y + removeRect.h / 2 - x.src_height / 2,
            x.src_width, x.src_height
        };
        SDL_RenderCopy(renderer, x.texture, nullptr, &xDst);
        SDL_DestroyTexture(x.texture);

        listY += entryH;
    }

    // scroll bar — thin strip on the right edge of the box
    if ((int)activePlayers.size() > CONFIG_VISIBLE) {
        int trackX = 726;
        int trackY = 153;
        int trackH = CONFIG_VISIBLE * entryH;
        int trackW = 8;

        // track background
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 180);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect track = { trackX, trackY, trackW, trackH };
        SDL_RenderFillRect(renderer, &track);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

        // thumb — sized proportional to visible vs total
        float ratio = (float)CONFIG_VISIBLE / activePlayers.size();
        int thumbH = max(20, (int)(trackH * ratio));
        float scrollRatio = (float)configScroll /
            max(1, (int)activePlayers.size() - CONFIG_VISIBLE);
        int thumbY = trackY + (int)((trackH - thumbH) * scrollRatio);

        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect thumb = { trackX, thumbY, trackW, thumbH };
        SDL_RenderFillRect(renderer, &thumb);
    }

    renderButton(assets.backBtn);
}

void Game::renderGame() {
    SDL_Rect bg_dst = { 0, 0, 800, 600 };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg_dst);

    // circle
    int cx = (baseWidth / 2)-10;
    int cy = static_cast<int>(circleCY);
    int count = circle.getSize();
    double step = (2.0 * M_PI) / count;
    // IT walker
    if (playPhase == PlayPhase::WALKING || playPhase == PlayPhase::CHOOSING_DIRECTION) {
        renderAnimation(assets.it_walk_bounce,
            static_cast<int>(itWalkerX),
            static_cast<int>(itWalkerY),
            circleScale * 3.0f);

        double step = (2.0 * M_PI) / count;
        int shadowX = cx + static_cast<int>(circleRadius * cos(step * itIndex)) - 16;
        int shadowY = cy + static_cast<int>(circleRadius * sin(step * itIndex)) - 16;
        float scale = circleScale * 3.0f;
        int size = static_cast<int>(32 * scale);

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
        SDL_Rect shadow = { shadowX, shadowY, size, size };
        SDL_RenderFillRect(renderer, &shadow);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    if (playPhase == PlayPhase::CHASING) {
        // goose chasing
        advanceAnimation(assets.goose_run);
        renderAnimation(assets.goose_run,
            static_cast<int>(gooseWalkerX),
            static_cast<int>(gooseWalkerY),
            circleScale * 1.5f);  // goose is 64px so smaller scale

        // IT running away
        advanceAnimation(assets.it_walk_bounce);
        renderAnimation(assets.it_walk_bounce,
            static_cast<int>(itWalkerX),
            static_cast<int>(itWalkerY),
            circleScale * 3.0f);

        renderTapMeter();

        Sprite tap = renderTextSprite("[ SPACE ] to tap!", { 0,0,0,255 }, 0);
        SDL_Rect tapDst = {
            baseWidth / 2 - tap.src_width / 2, 560,
            tap.src_width, tap.src_height
        };
        SDL_RenderCopy(renderer, tap.texture, nullptr, &tapDst);
        SDL_DestroyTexture(tap.texture);
    }
    for (int i = 0; i < count; i++) {
        if (i == itIndex && (playPhase == PlayPhase::WALKING ||
            playPhase == PlayPhase::CHOOSING_DIRECTION))
            continue;
        if (i == gooseIndex && playPhase == PlayPhase::CHASING)
            continue;

        int x = cx + static_cast<int>(circleRadius * cos(step * i)) - 16;
        int y = cy + static_cast<int>(circleRadius * sin(step * i)) - 16;
        bool highlighted = (playPhase == PlayPhase::SPINNING && i == spinIndex)
            || (playPhase != PlayPhase::SPINNING && i == selectedIndex);
        renderPlayer(activePlayers[i], x, y, highlighted);
    }
    // callout popup — goes here, after players, before UI panel
    if (activeCallout != CalloutType::NONE) {
        Uint32 elapsed = SDL_GetTicks() - calloutTimer;
        if (elapsed < CALLOUT_DURATION) {
            Uint8 alpha = 255;
            if (elapsed > CALLOUT_DURATION - 300)
                alpha = static_cast<Uint8>(255 * (CALLOUT_DURATION - elapsed) / 300.0f);

            SDL_Texture* tex = (activeCallout == CalloutType::DUCK)
                ? assets.duckCallout.texture
                : assets.gooseCallout.texture;

            int callW = (activeCallout == CalloutType::DUCK) ? 100 : 160;
            int callH = (activeCallout == CalloutType::DUCK) ? 100 : 82;
            int callX = calloutTargetX - callW / 2;
            int callY = calloutTargetY - callH;

            // clamp to screen
            callX = max(0, min(callX, baseWidth - callW));
            callY = max(0, min(callY, baseHeight - callH));

            SDL_Rect callDst = { callX, callY, callW, callH };
            SDL_SetTextureAlphaMod(tex, alpha);
            SDL_RenderCopy(renderer, tex, nullptr, &callDst);
            SDL_SetTextureAlphaMod(tex, 255);
        }
        else {
            activeCallout = CalloutType::NONE;
        }
    }


    // UI panel at bottom
    switch (playPhase) {

    case PlayPhase::CHOOSING_DIRECTION: {
        Sprite itLabel = renderTextSprite(
            activePlayers[itIndex].name + " is IT!", { 0,0,0,255 }, 0);
        SDL_Rect itLabelBoxDst = { 10, 20, 500, 71 };
        SDL_Rect labelDst = {
                50,
                20 + 71 / 2 - itLabel.src_height / 2,  // vertically centered in box
                itLabel.src_width,
                itLabel.src_height
        };        
        
        SDL_RenderCopy(renderer, assets.itLabelBox.texture, nullptr, &itLabelBoxDst);
        SDL_RenderCopy(renderer, itLabel.texture, nullptr, &labelDst);
        SDL_DestroyTexture(itLabel.texture);
        renderButton(assets.dirLeftBtn);
        renderButton(assets.dirRightBtn);
        break;
    }

    case PlayPhase::WALKING: {
        renderButton(assets.duckBtn);
        renderButton(assets.gooseBtn);
        break;
    }

    default: break;
    }
}

void Game::renderTapMeter() {
    int barX = 700;
    int barY = 80;
    int barH = 440;
    int leftW = 40;
    int rightW = 20;
    int gap = 6;

    int leftX = barX;
    int leftInnerX = leftX + 4;
    int leftInnerY = barY + 4;
    int leftInnerW = leftW - 8;
    int leftInnerH = barH - 8;

    // background
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_Rect leftBg = { leftX, barY, leftW, barH };
    SDL_RenderFillRect(renderer, &leftBg);

    // green fill — bottom up
    int fillH = static_cast<int>(leftInnerH * tapEnergy);
    SDL_SetRenderDrawColor(renderer, 60, 200, 60, 255);
    SDL_Rect fill = {
        leftInnerX,
        leftInnerY + leftInnerH - fillH,
        leftInnerW,
        fillH
    };
    SDL_RenderFillRect(renderer, &fill);

    // target line — from bottom, same system as fill
    int lineY = leftInnerY + leftInnerH - static_cast<int>(leftInnerH * targetPos);
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    SDL_RenderDrawLine(renderer, leftInnerX, lineY - 1, leftInnerX + leftInnerW, lineY - 1);
    SDL_RenderDrawLine(renderer, leftInnerX, lineY, leftInnerX + leftInnerW, lineY);
    SDL_RenderDrawLine(renderer, leftInnerX, lineY + 1, leftInnerX + leftInnerW, lineY + 1);

    // border
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &leftBg);

    // right bar — urgency drain
    int rightX = leftX + leftW + gap;
    int rightInnerX = rightX + 3;
    int rightInnerY = barY + 3;
    int rightInnerW = rightW - 6;
    int rightInnerH = barH - 6;

    SDL_SetRenderDrawColor(renderer, 80, 45, 10, 255);
    SDL_Rect rightBg = { rightX, barY, rightW, barH };
    SDL_RenderFillRect(renderer, &rightBg);

    float urgency = static_cast<float>(barChangeCount) / maxBarChanges;
    float drainLevel = 1.0f - urgency;
    int drainH = static_cast<int>(rightInnerH * drainLevel);
    Uint8 r = static_cast<Uint8>(255 * urgency);
    Uint8 g = static_cast<Uint8>(200 * (1.0f - urgency));
    SDL_SetRenderDrawColor(renderer, r, g, 0, 255);
    SDL_Rect drain = { rightInnerX, rightInnerY, rightInnerW, drainH };
    SDL_RenderFillRect(renderer, &drain);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rightBg);

    // pips
    int pipY = barY - 18;
    int pipW = (leftW + gap + rightW) / maxBarChanges - 2;
    for (int i = 0; i < maxBarChanges; i++) {
        SDL_SetRenderDrawColor(renderer,
            i < barChangeCount ? 60 : 120,
            i < barChangeCount ? 200 : 120,
            60, 255);
        SDL_Rect pip = { leftX + i * (pipW + 2), pipY, pipW, 10 };
        SDL_RenderFillRect(renderer, &pip);
    }

    // FASTER! warning
    if (urgency > 0.5f) {
        SDL_Color warnColor = { 255, static_cast<Uint8>(200 * (1.0f - urgency)), 0, 255 };
        Sprite warn = renderTextSprite("FASTER!", warnColor, 0);
        SDL_Rect warnDst = {
            leftX + (leftW + gap + rightW) / 2 - warn.src_width / 2,
            barY + barH + 8,
            warn.src_width,
            warn.src_height
        };
        SDL_RenderCopy(renderer, warn.texture, nullptr, &warnDst);
        SDL_DestroyTexture(warn.texture);
    }
}

void Game::renderRoundEnd() {
    SDL_Rect bg = { 0, 0, baseWidth, baseHeight };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg);

    SDL_Rect panelDst = { 18, 78, 591, 405 };
    SDL_RenderCopy(renderer, assets.cookingPotListUI.texture, nullptr, &panelDst);

    SDL_Rect resultDst = { 489, 44, 285, 154 };
    SDL_RenderCopy(renderer, assets.roundResultUI.texture, nullptr, &resultDst);

    if (cookingPotScroll > 0) {
        Sprite up = renderTextSprite("^ more ^", { 80,80,80,255 }, 0);
        SDL_Rect upDst = { 60, 148, up.src_width, up.src_height };
        SDL_RenderCopy(renderer, up.texture, nullptr, &upDst);
        SDL_DestroyTexture(up.texture);
    }

    int listX = 85;
    int listY = 214;
    int entryH = 36;
    int visibleEnd = min(cookingPotScroll + POT_VISIBLE, (int)cookingPot.size());

    for (int i = cookingPotScroll; i < visibleEnd; i++) {
        Sprite name = renderTextSprite(
            std::to_string(i + 1) + ".  " + cookingPot[i].name, { 0,0,0,255 }, 0);
        SDL_Rect nameDst = { listX, listY, name.src_width, name.src_height };
        SDL_RenderCopy(renderer, name.texture, nullptr, &nameDst);
        SDL_DestroyTexture(name.texture);
        listY += entryH;
    }

    if (visibleEnd < (int)cookingPot.size()) {
        Sprite down = renderTextSprite("v more v", { 80,80,80,255 }, 0);
        SDL_Rect downDst = { 60, listY, down.src_width, down.src_height };
        SDL_RenderCopy(renderer, down.texture, nullptr, &downDst);
        SDL_DestroyTexture(down.texture);
    }

    // effective remaining = circle size minus any pending elimination
    int effectiveRemaining = circle.getSize() - (pendingElimination ? 1 : 0);
    if (effectiveRemaining <= 1) {
        if (pendingElimination && eliminatedIndex >= 0) {
            addToPot(activePlayers[eliminatedIndex]);
            activePlayers.erase(activePlayers.begin() + eliminatedIndex);
            circle.remove(eliminatedIndex);
            if (itIndex > eliminatedIndex) itIndex--;
            pendingElimination = false;
            eliminatedIndex = -1;
        }
        winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
        state = GameState::GAME_OVER;
        needstoRedraw = true;
        return; // don't render round end at all
    }

    renderButton(assets.quitRoundBtn);

    if (effectiveRemaining > 1) {
        renderButton(assets.continueBtn);
    }
}
void Game::renderGameOver() {
    SDL_Rect bg = { 0, 0, baseWidth, baseHeight };
    SDL_RenderCopy(renderer, assets.bgTile.texture, nullptr, &bg);

    SDL_Rect winBoxDst = { baseWidth / 2 - 250, 120, 500, 196 };
    SDL_RenderCopy(renderer, assets.winnerBox.texture, nullptr, &winBoxDst);

    // winner name
    std::string displayName = winnerName.empty() ? "Nobody" : winnerName;
    Sprite name = renderTextSprite(displayName, { 0, 0, 0, 255 }, 0);
    SDL_Rect nameDst = {
        baseWidth / 2 - name.src_width / 2,
        120 + 196 / 2 - name.src_height / 2 + 30,
        name.src_width,
        name.src_height
    };
    SDL_RenderCopy(renderer, name.texture, nullptr, &nameDst);
    SDL_DestroyTexture(name.texture);

    renderButton(assets.playAgainBtn);
    renderButton(assets.gameOverQuitBtn);
}
//Event Handling
void Game::handleEvents(SDL_Event& e, bool& running) {
    while (SDL_PollEvent(&e)) {

        if (e.type == SDL_QUIT) {
            running = false;
            return;
        }

        if (state == GameState::CONFIGURE) {
            handleConfigureEvents(e, running);
            continue;
        }
        if (state == GameState::ROUND_END) {
            handleRoundEndEvents(e, running);
            continue;
        }
        if (state == GameState::GAME_OVER) {
            handleGameOverEvents(e, running);
            continue;
        }

        // mouse motion
        if (e.type == SDL_MOUSEMOTION) {
            int mx, my;
            getLogicalMouse(mx, my);

            switch (state) {
            case GameState::MAIN_MENU:
                assets.playButton.isHovered = isHovered(assets.playButton.rect, mx, my);
                assets.quitButton.isHovered = isHovered(assets.quitButton.rect, mx, my);
                if (assets.playButton.isHovered) menuSelection = 0;
                if (assets.quitButton.isHovered) menuSelection = 1;
                break;

            case GameState::PLAYER_SETUP:
                assets.addPlayerBtn.isHovered = isHovered(assets.addPlayerBtn.rect, mx, my);
                assets.uploadFileBtn.isHovered = isHovered(assets.uploadFileBtn.rect, mx, my);
                assets.startBtn.isHovered = isHovered(assets.startBtn.rect, mx, my);
                assets.configureBtn.isHovered = isHovered(assets.configureBtn.rect, mx, my);
                break;

            case GameState::PLAYING:
                switch (playPhase) {
                case PlayPhase::CHOOSING_DIRECTION:
                    assets.dirLeftBtn.isHovered = isHovered(assets.dirLeftBtn.rect, mx, my);
                    assets.dirRightBtn.isHovered = isHovered(assets.dirRightBtn.rect, mx, my);
                    break;
                case PlayPhase::WALKING:
                    assets.duckBtn.isHovered = isHovered(assets.duckBtn.rect, mx, my);
                    assets.gooseBtn.isHovered = isHovered(assets.gooseBtn.rect, mx, my);
                    break;
                default: break;
                }
                break;

            default: break;
            }
            needstoRedraw = true;
        }

        // mouse clicks
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx, my;
            getLogicalMouse(mx, my);

            switch (state) {
            case GameState::SPLASH:
                state = GameState::MAIN_MENU;
                break;

            case GameState::MAIN_MENU:
                if (isHovered(assets.playButton.rect, mx, my)) {
                    state = GameState::PLAYER_SETUP;
                    SDL_StartTextInput();
                }
                if (isHovered(assets.quitButton.rect, mx, my)) running = false;
                break;

            case GameState::PLAYER_SETUP:
                if (isHovered(assets.addPlayerBtn.rect, mx, my) && !currentInput.empty()) {
                    createPlayer(currentInput);
                    currentInput = "";
                }
                if (isHovered(assets.uploadFileBtn.rect, mx, my)) {
                    std::string path;
                    if (openFileDialog(path))
                        loadPlayersFromFile(path);
                }
                if (isHovered(assets.configureBtn.rect, mx, my) && circle.getSize() > 0) {
                    configScroll = 0;
                    state = GameState::CONFIGURE;
                }
                if (isHovered(assets.startBtn.rect, mx, my) && circle.getSize() >= 2) {
                    SDL_StopTextInput();
                    createCircle();
                }
                break;

            case GameState::PLAYING:
                switch (playPhase) {
                case PlayPhase::CHOOSING_DIRECTION:
                    if (isHovered(assets.dirLeftBtn.rect, mx, my)) {
                        goingClockwise = false;
                        selectedIndex = itIndex;
                        selectedIndex = (selectedIndex - 1 + circle.getSize()) % circle.getSize();
                        updateITWalkerTarget();
                        playPhase = PlayPhase::WALKING;
                    }
                    if (isHovered(assets.dirRightBtn.rect, mx, my)) {
                        goingClockwise = true;
                        selectedIndex = itIndex;
                        selectedIndex = (selectedIndex + 1) % circle.getSize();
                        updateITWalkerTarget();
                        playPhase = PlayPhase::WALKING;
                    }
                    break;

                case PlayPhase::WALKING:
                    if (isHovered(assets.duckBtn.rect, mx, my)) {
                        selectedIndex = goingClockwise
                            ? (selectedIndex + 1) % circle.getSize()
                            : (selectedIndex - 1 + circle.getSize()) % circle.getSize();
                        if (selectedIndex == itIndex)
                            selectedIndex = goingClockwise
                            ? (selectedIndex + 1) % circle.getSize()
                            : (selectedIndex - 1 + circle.getSize()) % circle.getSize();
                        updateITWalkerTarget();
                        activeCallout = CalloutType::DUCK;
                        calloutTimer = SDL_GetTicks();
                        calloutTargetX = static_cast<int>(itWalkerX);
                        calloutTargetY = static_cast<int>(itWalkerY) - 60;
                    }
                    if (isHovered(assets.gooseBtn.rect, mx, my) && selectedIndex != itIndex) {
                        gooseIndex = selectedIndex;
                        setRole(activePlayers[gooseIndex], GOOSE);
                        tapEnergy = 0.0f;
                        barChangeCount = 0;
                        maxBarChanges = min(8, circle.getSize() * 2);
                        targetPos = 0.25f + static_cast<float>(rand()) /
                            (static_cast<float>(RAND_MAX / 0.65f));
                        chaseActive = true;
                        chaseStarted = true;
                        chaseStartTime = SDL_GetTicks();

                        selectedIndex = itIndex;
                        updateITWalkerTarget();
                        itWalkerX = itWalkerTargetX;
                        itWalkerY = itWalkerTargetY;

                        gooseWalkerIndex = goingClockwise
                            ? (gooseIndex - 1 + circle.getSize()) % circle.getSize()
                            : (gooseIndex + 1) % circle.getSize();
                        updateGooseWalkerTarget();
                        gooseWalkerX = gooseWalkerTargetX;
                        gooseWalkerY = gooseWalkerTargetY;

                        gooseLastStep = SDL_GetTicks();
                        itChaseLastStep = SDL_GetTicks();

                        playPhase = PlayPhase::CHASING;
                        activeCallout = CalloutType::GOOSE;
                        calloutTimer = SDL_GetTicks();
                        calloutTargetX = static_cast<int>(itWalkerX);
                        calloutTargetY = static_cast<int>(itWalkerY) - 60;
                    }
                    break;

                default: break;
                }
                break;

            default: break;
            }
            needstoRedraw = true;
        }

        // text input
        if (e.type == SDL_TEXTINPUT) {
            if (state == GameState::PLAYER_SETUP) {
                if (currentInput.length() < MAX_NAME_LENGTH) {
                    currentInput += e.text.text;
                    needstoRedraw = true;
                }
            }
        }

        // keydown
        if (e.type == SDL_KEYDOWN) {
            switch (state) {
            case GameState::SPLASH:
                state = GameState::MAIN_MENU;
                needstoRedraw = true;
                break;

            case GameState::MAIN_MENU:
                if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_LEFT)
                    menuSelection = (menuSelection - 1 + 2) % 2;
                if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_RIGHT)
                    menuSelection = (menuSelection + 1) % 2;

                assets.playButton.isHovered = (menuSelection == 0);
                assets.quitButton.isHovered = (menuSelection == 1);

                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (menuSelection == 0) { state = GameState::PLAYER_SETUP; SDL_StartTextInput(); }
                    if (menuSelection == 1) running = false;
                }
                needstoRedraw = true;
                break;

            case GameState::PLAYER_SETUP:
                if (e.key.keysym.sym == SDLK_BACKSPACE && !currentInput.empty())
                    currentInput.pop_back();
                if (e.key.keysym.sym == SDLK_RETURN && !currentInput.empty()) {
                    createPlayer(currentInput);
                    currentInput = "";
                }
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_StopTextInput();
                    state = GameState::MAIN_MENU;
                }
                needstoRedraw = true;
                break;

            case GameState::PLAYING:
                handleGameEvents(e, running);
                needstoRedraw = true;
                break;

            default:
                if (e.key.keysym.sym == SDLK_ESCAPE) running = false;
                break;
            }
        }
    }
}

void Game::handleSetupEvents(SDL_Event& e, bool& running) {
    if (e.type == SDL_TEXTINPUT) {
        if (currentInput.length() < MAX_NAME_LENGTH) {
            currentInput += e.text.text;
            needstoRedraw = true;
        }
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_BACKSPACE && !currentInput.empty()) {
            currentInput.pop_back();
            needstoRedraw = true;
        }
        if (e.key.keysym.sym == SDLK_RETURN && !currentInput.empty()) {
            createPlayer(currentInput);
            currentInput = "";
            needstoRedraw = true;
        }
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            SDL_StopTextInput();
            state = GameState::MAIN_MENU;
            needstoRedraw = true;
        }
    }

    if (e.type == SDL_MOUSEMOTION) {
        int mx, my;
        getLogicalMouse(mx, my);
        assets.addPlayerBtn.isHovered = isHovered(assets.addPlayerBtn.rect, mx, my);
        assets.startBtn.isHovered = isHovered(assets.startBtn.rect, mx, my);
        needstoRedraw = true;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        getLogicalMouse(mx, my);

        if (isHovered(assets.addPlayerBtn.rect, mx, my) && !currentInput.empty()) {
            createPlayer(currentInput);
            currentInput = "";
            needstoRedraw = true;
        }
        if (isHovered(assets.startBtn.rect, mx, my) && circle.getSize() >= 2) {
            SDL_StopTextInput();
            createCircle();
            needstoRedraw = true;
        }
    }
}

void Game::handleConfigureEvents(SDL_Event& e, bool& running) {
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0)
            configScroll = max(0, configScroll - 1);
        if (e.wheel.y < 0)
            configScroll = min(
                (int)activePlayers.size() - CONFIG_VISIBLE, configScroll + 1);
        needstoRedraw = true;
    }

    if (e.type == SDL_MOUSEMOTION) {
        int mx, my;
        getLogicalMouse(mx, my);
        assets.backBtn.isHovered = isHovered(assets.backBtn.rect, mx, my);
        needstoRedraw = true;
    }

    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        getLogicalMouse(mx, my);

        // back button
        if (isHovered(assets.backBtn.rect, mx, my)) {
            state = GameState::PLAYER_SETUP;
            SDL_StartTextInput();
            needstoRedraw = true;
            return;
        }

        // check remove button clicks — recalculate row rects
        int listY = 153;
        int entryH = 38;
        int visibleEnd = min(configScroll + CONFIG_VISIBLE, (int)activePlayers.size());

        for (int i = configScroll; i < visibleEnd; i++) {
            SDL_Rect removeRect = { 448, listY, 50, 28 };
            if (isHovered(removeRect, mx, my)) {
                // remove from activePlayers and circle
                activePlayers.erase(activePlayers.begin() + i);
                circle.remove(i);
                // clamp scroll in case list shrank
                configScroll = max(0,
                    min(configScroll, (int)activePlayers.size() - CONFIG_VISIBLE));
                needstoRedraw = true;
                return;
            }
            listY += entryH;
        }
    }

    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP)
            configScroll = max(0, configScroll - 1);
        if (e.key.keysym.sym == SDLK_DOWN)
            configScroll = min(
                (int)activePlayers.size() - CONFIG_VISIBLE, configScroll + 1);
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            state = GameState::PLAYER_SETUP;
            SDL_StartTextInput();
        }
        needstoRedraw = true;
    }
}

void Game::handleGameEvents(SDL_Event& e, bool& running) {
    if (e.type != SDL_KEYDOWN) return;

    switch (playPhase) {

    case PlayPhase::CHOOSING_DIRECTION:
        if (e.key.keysym.sym == SDLK_LEFT) {
            goingClockwise = false;
            selectedIndex = itIndex;
            selectedIndex = (selectedIndex - 1 + circle.getSize()) % circle.getSize();
            updateITWalkerTarget();
            playPhase = PlayPhase::WALKING;
        }
        if (e.key.keysym.sym == SDLK_RIGHT) {
            goingClockwise = true;
            selectedIndex = itIndex;
            selectedIndex = (selectedIndex + 1) % circle.getSize();
            updateITWalkerTarget();
            playPhase = PlayPhase::WALKING;
        }
        break;

    case PlayPhase::WALKING:
        if (e.key.keysym.sym == SDLK_LEFT) {
            selectedIndex = goingClockwise
                ? (selectedIndex - 1 + circle.getSize()) % circle.getSize()
                : (selectedIndex + 1) % circle.getSize();
            if (selectedIndex == itIndex)
                selectedIndex = goingClockwise
                ? (selectedIndex - 1 + circle.getSize()) % circle.getSize()
                : (selectedIndex + 1) % circle.getSize();
            updateITWalkerTarget();
            activeCallout = CalloutType::DUCK;
            calloutTimer = SDL_GetTicks();
            calloutTargetX = static_cast<int>(itWalkerX);
            calloutTargetY = static_cast<int>(itWalkerY) - 60;
        }
        if (e.key.keysym.sym == SDLK_RIGHT) {
            selectedIndex = goingClockwise
                ? (selectedIndex + 1) % circle.getSize()
                : (selectedIndex - 1 + circle.getSize()) % circle.getSize();
            if (selectedIndex == itIndex)
                selectedIndex = goingClockwise
                ? (selectedIndex + 1) % circle.getSize()
                : (selectedIndex - 1 + circle.getSize()) % circle.getSize();
            updateITWalkerTarget();
            activeCallout = CalloutType::DUCK;
            calloutTimer = SDL_GetTicks();
            calloutTargetX = static_cast<int>(itWalkerX);
            calloutTargetY = static_cast<int>(itWalkerY) - 60;
        }
        if (e.key.keysym.sym == SDLK_RETURN && selectedIndex != itIndex) {
            gooseIndex = selectedIndex;
            setRole(activePlayers[gooseIndex], GOOSE);
            tapEnergy = 0.0f;
            barChangeCount = 0;
            maxBarChanges = min(8, circle.getSize() * 2);
            targetPos = 0.25f + static_cast<float>(rand()) /
                (static_cast<float>(RAND_MAX / 0.65f));
            chaseActive = true;
            chaseStarted = true;
            chaseStartTime = SDL_GetTicks();

            selectedIndex = itIndex;
            updateITWalkerTarget();
            itWalkerX = itWalkerTargetX;
            itWalkerY = itWalkerTargetY;

            gooseWalkerIndex = goingClockwise
                ? (gooseIndex - 1 + circle.getSize()) % circle.getSize()
                : (gooseIndex + 1) % circle.getSize();
            updateGooseWalkerTarget();
            gooseWalkerX = gooseWalkerTargetX;
            gooseWalkerY = gooseWalkerTargetY;

            gooseLastStep = SDL_GetTicks();
            itChaseLastStep = SDL_GetTicks();

            playPhase = PlayPhase::CHASING;
            activeCallout = CalloutType::GOOSE;
            calloutTimer = SDL_GetTicks();
            calloutTargetX = static_cast<int>(itWalkerX);
            calloutTargetY = static_cast<int>(itWalkerY) - 60;
        }
        break;

    case PlayPhase::CHASING:
        if (e.key.keysym.sym == SDLK_SPACE || e.key.keysym.sym == SDLK_UP) {
            float prevEnergy = tapEnergy;
            tapEnergy += meterFill;
            if (tapEnergy > 1.0f) tapEnergy = 1.0f;

            float zoneSize = 0.12f;
            float zoneBottom = targetPos - zoneSize / 2;
            float zoneTop = targetPos + zoneSize / 2;

            // hit if we crossed through or landed in the zone
            bool hit = (prevEnergy < zoneBottom && tapEnergy >= zoneBottom)
                || (tapEnergy >= zoneBottom && tapEnergy <= zoneTop);

            if (hit) {
                barChangeCount++;
                tapEnergy = 0.0f;
                targetPos = 0.1f + static_cast<float>(rand()) /
                    (static_cast<float>(RAND_MAX / 0.8f));

                if (barChangeCount >= maxBarChanges) {
                    addToPot(activePlayers[itIndex]);
                    int taggedIndex = itIndex;
                    setRole(activePlayers[taggedIndex], DUCK);
                    pendingElimination = true;
                    eliminatedIndex = taggedIndex;
                    itIndex = gooseIndex;
                    setRole(activePlayers[itIndex], IT);
                    chaseActive = false;
                    updateITWalkerTarget();
                    playPhase = PlayPhase::CHOOSING_DIRECTION;
                    if (circle.getSize() - 1 <= 0) {
                        activePlayers.erase(activePlayers.begin() + eliminatedIndex);
                        circle.remove(eliminatedIndex);
                        if (itIndex > eliminatedIndex) itIndex--;
                        pendingElimination = false;
                        eliminatedIndex = -1;
                        winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
                        state = GameState::GAME_OVER;
                    }
                    else {
                        state = GameState::ROUND_END;
                    }
                }
                else {
                    float successThreshold = maxBarChanges * 0.7f;
                    if (barChangeCount >= successThreshold) {
                        int oldIt = itIndex;
                        itIndex = gooseIndex;
                        selectedIndex = oldIt;
                        setRole(activePlayers[itIndex], IT);
                        setRole(activePlayers[selectedIndex], DUCK);
                        chaseActive = false;
                        updateITWalkerTarget();
                        playPhase = PlayPhase::CHOOSING_DIRECTION;
                        if (circle.getSize() - (pendingElimination ? 1 : 0) <= 1) {
                            winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
                            state = GameState::GAME_OVER;
                        }
                        else {
                            state = GameState::ROUND_END;
                        }
                    }
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

void Game::handleRoundEndEvents(SDL_Event& e, bool& running) {
    if (e.type == SDL_MOUSEWHEEL) {
        if (e.wheel.y > 0)
            cookingPotScroll = max(0, cookingPotScroll - 1);
        if (e.wheel.y < 0)
            cookingPotScroll = min((int)cookingPot.size() - POT_VISIBLE, cookingPotScroll + 1);
        needstoRedraw = true;
    }
    if (e.type == SDL_MOUSEMOTION) {
        int mx, my;
        getLogicalMouse(mx, my);
        assets.continueBtn.isHovered = isHovered(assets.continueBtn.rect, mx, my);
        assets.quitRoundBtn.isHovered = isHovered(assets.quitRoundBtn.rect, mx, my);
        needstoRedraw = true;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        getLogicalMouse(mx, my);
        if (isHovered(assets.continueBtn.rect, mx, my)) {
            if (circle.getSize() - (pendingElimination ? 1 : 0) <= 1) {
                if (pendingElimination && eliminatedIndex >= 0) {
                    activePlayers.erase(activePlayers.begin() + eliminatedIndex);
                    circle.remove(eliminatedIndex);
                    pendingElimination = false;
                    eliminatedIndex = -1;
                }
                winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
                state = GameState::GAME_OVER;
            }
            else {
                startNextRound();
            }
            needstoRedraw = true;
        }
        if (isHovered(assets.quitRoundBtn.rect, mx, my)) {
            resetGame();
            state = GameState::SPLASH;
        }
        needstoRedraw = true;
    }
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_UP)
            cookingPotScroll = max(0, cookingPotScroll - 1);
        if (e.key.keysym.sym == SDLK_DOWN)
            cookingPotScroll = min((int)cookingPot.size() - POT_VISIBLE, cookingPotScroll + 1);
        if (e.key.keysym.sym == SDLK_RETURN) {
            if (circle.getSize() - (pendingElimination ? 1 : 0) <= 1) {
                if (pendingElimination && eliminatedIndex >= 0) {
                    activePlayers.erase(activePlayers.begin() + eliminatedIndex);
                    circle.remove(eliminatedIndex);
                    pendingElimination = false;
                    eliminatedIndex = -1;
                }
                winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
                state = GameState::GAME_OVER;
            }
            else {
                startNextRound();
            }
        }
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            resetGame();
            state = GameState::SPLASH;
        }
        needstoRedraw = true;
    }
}

void Game::handleGameOverEvents(SDL_Event& e, bool& running) {
    if (e.type == SDL_MOUSEMOTION) {
        int mx, my;
        getLogicalMouse(mx, my);
        assets.playAgainBtn.isHovered = isHovered(assets.playAgainBtn.rect, mx, my);
        assets.gameOverQuitBtn.isHovered = isHovered(assets.gameOverQuitBtn.rect, mx, my);
        needstoRedraw = true;
    }
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mx, my;
        getLogicalMouse(mx, my);
        if (isHovered(assets.playAgainBtn.rect, mx, my)) {
            resetGame();
        }
        if (isHovered(assets.gameOverQuitBtn.rect, mx, my))
            state = GameState::SPLASH;
        needstoRedraw = true;
    }
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_RETURN) {
            resetGame();
        }
        if (e.key.keysym.sym == SDLK_ESCAPE)
            state = GameState::SPLASH;
        needstoRedraw = true;
    }
}

void Game::updatePos() {
    if (playPhase != PlayPhase::CHASING) return;
    if (SDL_GetTicks() - chaseStartTime < 1000) return;

    float urgency = static_cast<float>(barChangeCount) / maxBarChanges;
    meterDecay = 0.0008f + (urgency * 0.0015f);

    tapEnergy -= meterDecay;
    if (tapEnergy < 0.0f) tapEnergy = 0.0f;

    if (tapEnergy <= 0.0f && chaseActive) {
        // bounds check before doing anything
        if (itIndex < 0 || itIndex >= (int)activePlayers.size()) return;
        if (gooseIndex < 0 || gooseIndex >= (int)activePlayers.size()) return;

        addToPot(activePlayers[itIndex]);
        setRole(activePlayers[itIndex], DUCK);
        int savedEliminated = itIndex;
        int savedNewIt = gooseIndex;

        // adjust savedNewIt if it will shift after erase
        pendingElimination = true;
        eliminatedIndex = savedEliminated;
        itIndex = savedNewIt;
        setRole(activePlayers[itIndex], IT);
        chaseActive = false;
        updateITWalkerTarget();
        playPhase = PlayPhase::CHOOSING_DIRECTION;

        if (circle.getSize() - 1 <= 0) {
            activePlayers.erase(activePlayers.begin() + eliminatedIndex);
            circle.remove(eliminatedIndex);
            if (itIndex > eliminatedIndex) itIndex--;
            pendingElimination = false;
            eliminatedIndex = -1;
            winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
            state = GameState::GAME_OVER;
        }
        else {
            state = GameState::ROUND_END;
        }
    }

    needstoRedraw = true;
}
Game::Player Game::createPlayer(std::string name) {
    if (circle.getSize() >= 20) return Player(); // max 20 players

    Player p;
    p.name = name;
    p.status = DUCK;
    p.playerID = static_cast<int>(circle.getSize()); // placeholder ID logic
    p.nameSprite = renderTextSprite(name, {0,0,0,255 }, 0); // white
    registeredTextures.push_back(p.nameSprite.texture);

    circle.insert(p.name, p.playerID);
    activePlayers.push_back(p);

    circle.display(std::cout);

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
    pendingItIndex = rand() % count;   // store separately
    itIndex = 0;                        // reset itIndex to 0 during spin
    spinIndex = 0;
    spinSpeed = 50;
    spinStepsLeft = 20 + (count * 3) + pendingItIndex;
    isSpinning = true;
    playPhase = PlayPhase::SPINNING;
    state = GameState::PLAYING;
}

//Main Game Loop
void Game::play() {
    bool running = true;
    SDL_Event e;
    Uint32 now = 0;

    while (running) {
        now = SDL_GetTicks();

        handleEvents(e, running);

        float deltaTime = (now - fadeTimer) / 1000.0f;
        fadeTimer = now;

        // splash fade
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

        // cursor blink
        if (now - blinkTimer >= BLINK_INTERVAL) {
            blinkVisible = !blinkVisible;
            blinkTimer = now;
            needstoRedraw = true;
        }

        // chase — goose and IT walker movement
        if (state == GameState::PLAYING && playPhase == PlayPhase::CHASING) {
            Uint32 gooseStepInterval = 1200;
            if (now - gooseLastStep >= gooseStepInterval) {
                if (gooseWalkerIndex != itIndex) {
                    gooseWalkerIndex = goingClockwise
                        ? (gooseWalkerIndex + 1) % circle.getSize()
                        : (gooseWalkerIndex - 1 + circle.getSize()) % circle.getSize();
                    updateGooseWalkerTarget();

                    if (gooseWalkerIndex == itIndex) {
                        if (itIndex >= 0 && itIndex < (int)activePlayers.size() &&
                            gooseIndex >= 0 && gooseIndex < (int)activePlayers.size()) {
                            addToPot(activePlayers[itIndex]);
                            int taggedIndex = itIndex;
                            setRole(activePlayers[taggedIndex], DUCK);
                            pendingElimination = true;
                            eliminatedIndex = taggedIndex;
                            itIndex = gooseIndex;
                            setRole(activePlayers[itIndex], IT);
                            chaseActive = false;
                            playPhase = PlayPhase::CHOOSING_DIRECTION;
                            updateITWalkerTarget();

                            if (circle.getSize() - 1 <= 0) {
                                activePlayers.erase(activePlayers.begin() + eliminatedIndex);
                                circle.remove(eliminatedIndex);
                                if (itIndex > eliminatedIndex) itIndex--;
                                pendingElimination = false;
                                eliminatedIndex = -1;
                                winnerName = activePlayers.empty() ? "Nobody" : activePlayers[0].name;
                                state = GameState::GAME_OVER;
                            }
                            else {
                                state = GameState::ROUND_END;
                            }
                        }
                    }
                }
                gooseLastStep = now;
            }

            Uint32 itStepInterval = 900;
            if (now - itChaseLastStep >= itStepInterval) {
                selectedIndex = goingClockwise
                    ? (selectedIndex + 1) % circle.getSize()
                    : (selectedIndex - 1 + circle.getSize()) % circle.getSize();
                updateITWalkerTarget();
                itChaseLastStep = now;
            }
        }

        // gameplay update
        if (state == GameState::PLAYING) {
            updatePos();

            // spin
            if (playPhase == PlayPhase::SPINNING) {
                if (now - spinLastStep >= (Uint32)spinSpeed) {
                    spinIndex = (spinIndex + 1) % circle.getSize();
                    spinStepsLeft--;

                    if (spinStepsLeft < 10) spinSpeed += 30;
                    if (spinStepsLeft < 5)  spinSpeed += 50;

                    if (spinStepsLeft <= 0) {
                        itIndex = pendingItIndex;
                        setRole(activePlayers[itIndex], IT);
                        isSpinning = false;
                        updateITWalkerTarget();
                        itWalkerX = itWalkerTargetX;
                        itWalkerY = itWalkerTargetY;
                        circleTargetRadius = 110.0f;
                        circleTargetCY = 200.0f;
                        circleTargetScale = 0.6f;
                        playPhase = PlayPhase::CHOOSING_DIRECTION;
                    }
                    spinLastStep = now;
                }
            }

            // tween
            float lerpSpeed = 5.0f * deltaTime;
            circleRadius = lerp(circleRadius, circleTargetRadius, lerpSpeed);
            circleCY = lerp(circleCY, circleTargetCY, lerpSpeed);
            circleScale = lerp(circleScale, circleTargetScale, lerpSpeed);
            itWalkerX = lerp(itWalkerX, itWalkerTargetX, lerpSpeed);
            itWalkerY = lerp(itWalkerY, itWalkerTargetY, lerpSpeed);
            gooseWalkerX = lerp(gooseWalkerX, gooseWalkerTargetX, lerpSpeed);
            gooseWalkerY = lerp(gooseWalkerY, gooseWalkerTargetY, lerpSpeed);

            needstoRedraw = true;
        }

        // render
        if (needstoRedraw) {
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderClear(renderer);

            switch (state) {
            case GameState::SPLASH:       renderSplash();    break;
            case GameState::MAIN_MENU:    renderMM();        break;
            case GameState::PLAYER_SETUP: renderSetup();     break;
            case GameState::CONFIGURE:    renderConfigure(); break;
            case GameState::PLAYING:      renderGame();      break;
            case GameState::ROUND_END:    renderRoundEnd();  break;
            case GameState::GAME_OVER:    renderGameOver();  break;
            }

            SDL_RenderPresent(renderer);
            needstoRedraw = false;
        }

        SDL_Delay(16);
    }
}
