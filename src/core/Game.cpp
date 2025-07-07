#include "core/Game.h"
#include "core/State.h"
#include "utils/Resources.h"
#include "utils/InputManager.h"
#include "components/Camera.h"
#include "states/TitleState.h"

#include <bits/stdc++.h>

Game* Game::instance;
Game& Game::GetInstance() {
    if (Game::instance == nullptr) {
        Game::instance = new Game("Pale Mirror", 1200, 900);
    }
    return *Game::instance;
}

Game::Game(const char* title, int width, int height) {
    if (Game::instance != nullptr) {
        return;
    }

    Game::instance = this;
    srand(time(NULL));

    // Inicializar SDL
    int init_error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER);
    if (init_error) {
        std::cout << "Erro ao inicializar SDL: ";
        std::cout << SDL_GetError()  << std::endl;
    }
    // Inicializar SDL_image
    init_error = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    if (init_error != (IMG_INIT_JPG | IMG_INIT_PNG)) {
        std::cout << "Erro ao inicializar SDL_image: ";
        std::cout << IMG_GetError()  << std::endl;
    }
    // Inicializar SDL_mixer
    init_error = Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG);
    if (init_error != (MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG)) {
        std::cout << "Erro ao inicializar SDL_mixer: ";
        std::cout << Mix_GetError() << std::endl;
    }
    init_error = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    if (init_error) {
        std::cout << "Erro ao executar Mix_OpenAudio: ";
        std::cout << Mix_GetError() << std::endl;
    }
    init_error = Mix_AllocateChannels(32);
    if (init_error != 32) {
        std::cout << "Erro ao executar Mix_AllocateChannels: ";
        std::cout << Mix_GetError() << std::endl;
    }
    // Initializar SDL_ttf
    init_error = TTF_Init();
    if (init_error) {
        std::cout << "Erro ao inicializar SDL_TTF: ";
        std::cout << TTF_GetError() << std::endl;
    }
    // Inicializar janela
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        std::cout << "Erro ao inicializar janela: ";
        std::cout << SDL_GetError() << std::endl;
    }
    // Inicializar renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Erro ao inicializar renderer: ";
        std::cout << SDL_GetError() << std::endl;
    }

    storedState = nullptr;
    frameStart = SDL_GetTicks();
    dt = 0;
}

Game::~Game() {
    if (storedState != nullptr) {
        delete storedState;
    }
    while (!stateStack.empty()) stateStack.pop();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

State& Game::GetCurrentState() {
    return *stateStack.top();
}

void Game::Push(State *state) {
    storedState = state;
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Run() {
    if (storedState == nullptr) return;
    stateStack.emplace(storedState);
    storedState = nullptr;
    stateStack.top()->Start();

    while (!stateStack.empty() && !stateStack.top()->QuitRequested()) {
        if (stateStack.top()->PopRequested()) {
            stateStack.pop();
            if (!stateStack.empty()) stateStack.top()->Resume();
        }

        if (storedState != nullptr) {
            stateStack.top()->Pause();
            stateStack.emplace(storedState);
            storedState = nullptr;
            stateStack.top()->Start();
        }

        if (!stateStack.empty()) {
            CalculateDeltaTime();
            SDL_RenderClear(renderer);
            INPUT_MANAGER.Update();
            stateStack.top()->Update(dt);
            stateStack.top()->Render();
            SDL_RenderPresent(renderer);
            SDL_Delay(33);
        }
    }

    Resources::ClearAll();
}

void Game::CalculateDeltaTime() {
    int oldFrameStart = frameStart;
    frameStart = SDL_GetTicks();
    dt = (frameStart - oldFrameStart) / 1000.0;
}
