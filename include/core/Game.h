#ifndef GAME_H
#define GAME_H

#define GAME_RENDERER Game::GetInstance().GetRenderer()
#define CURRENT_STATE Game::GetInstance().GetCurrentState()

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stack>

#include "utils/InputManager.h"
#include "utils/Resources.h"
#include "core/State.h"

class Game
{
public:
    ~Game();

    void Run();

    SDL_Renderer* GetRenderer();
    float GetDeltaTime();

    State& GetCurrentState();
    void Push(State* state);

    static Game& GetInstance();
private:
    Game(const char* title, int width, int height);
    static Game* instance;

    SDL_Window* window;
    SDL_Renderer* renderer;

    std::stack<std::unique_ptr<State>> stateStack;
    State* storedState;

    int frameStart;
    float dt;
    void CalculateDeltaTime();
};

#endif // GAME_H
