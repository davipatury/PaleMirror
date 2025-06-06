#include "states/TitleState.h"
#include "states/StageState.h"
#include "states/LoadState.h"

TitleState::TitleState() {
    started = false;
    quitRequested = false;
    popRequested = false;

    selectedOption = 0;

    blinkTimer.Restart();
    blinkVisible = true;

    titleMusic.Open("Recursos/audio/Title.wav");
    titleMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/Home.jpeg", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets() {}

void TitleState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::Update(float dt) {
    // Quit requested
    if (INPUT_MANAGER.IsKeyDown(ESCAPE_KEY) || INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    if (INPUT_MANAGER.IsKeyDown(SDLK_SPACE)) {
        Game::GetInstance().Push(new StageState());
    }

    if (INPUT_MANAGER.KeyPress(LEFT_ARROW_KEY) or INPUT_MANAGER.KeyPress(SDLK_a)) {
        if(selectedOption != 0) blinkTimer.Restart();
        selectedOption = 0;
    }

    if (INPUT_MANAGER.KeyPress(RIGHT_ARROW_KEY) or INPUT_MANAGER.KeyPress(SDLK_d)) {
        if(selectedOption != 1) blinkTimer.Restart();
        selectedOption = 1;
    }

    if (INPUT_MANAGER.KeyPress(SDLK_RETURN) or INPUT_MANAGER.IsKeyDown(SDLK_SPACE)) {
        if (selectedOption == 0) {
            Game::GetInstance().Push(new StageState());
        }
        else {
            Game::GetInstance().Push(new LoadState());
        }
    }

    if(blinkTimer.Get() >= BLINK_INTERVAL){
        blinkTimer.Restart();
        blinkVisible = !blinkVisible;
    }

    blinkTimer.Update(dt);

    UpdateArray(dt);
}

void TitleState::Render() {
    RenderArray();

    SDL_Rect highlightRect;
    if (selectedOption == 0) {
        highlightRect.x = 38;
        highlightRect.y = 643;
        highlightRect.w = 545;
        highlightRect.h = 77;
    }
    else {
        highlightRect.x = 622;
        highlightRect.y = 647;
        highlightRect.w = 545;
        highlightRect.h = 77;
    }

    if(blinkVisible){
        SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, 255);
        SDL_RenderDrawRect(GAME_RENDERER, &highlightRect);
    }

}

void TitleState::Pause() {
    titleMusic.Stop();
}
void TitleState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
    titleMusic.Play();
}
