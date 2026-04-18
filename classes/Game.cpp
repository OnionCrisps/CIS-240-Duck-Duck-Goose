#include "Game.h"
#include <iostream>

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
        screenWidth, screenHeight,
        SDL_WINDOW_SHOWN
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

    return true;
}

void Game::shutdownSDL() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
}

// ─── Event Handling ─────────────────────────────────────────────────────────

void Game::handleEvents(SDL_Event& e, bool& running) {
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                running = false;
                break;
                // add more key handling here as needed
            default:
                break;
            }
        }
    }
}

// ─── Rendering ──────────────────────────────────────────────────────────────

void Game::render() {
    // clear screen to a dark background
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);

    // TODO: draw circle of players
    // TODO: draw UI / menus
    // TODO: draw pot

    SDL_RenderPresent(renderer);
}

// ─── Game Logic ─────────────────────────────────────────────────────────────

Game::Player Game::createPlayer(std::string name) {
    Player p;
    p.name = name;
    p.status = TAPPER;
    p.playerID = static_cast<int>(cookingPot.size()); // placeholder ID logic
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

void Game::renderCircle() const {
    // TODO: walk DCList and render each node
}

void Game::renderPot() const {
    // TODO: render eliminated players
}

void Game::buildMenu() {
    // TODO: render a start menu, collect player names, etc.
}

void Game::createCircle() {
    // TODO: take players from cookingPot and insert into DCList
}

// ─── Main Game Loop ─────────────────────────────────────────────────────────

void Game::play() {
    buildMenu(); // setup phase — collect players, build circle

    bool running = true;
    SDL_Event e;

    while (running) {
        handleEvents(e, running);

        // game logic update here
        // e.g. check win conditions, advance round, etc.

        render();
    }
}