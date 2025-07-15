#include "states/LoadState.h"

LoadState::LoadState() {
    started = false;
    quitRequested = false;
    popRequested = false;

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/menu/Load.jpeg", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);
}

LoadState::~LoadState(){
    objectArray.clear();
}

void LoadState::LoadAssets() {}

void LoadState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void LoadState::Update(float dt) {
    if (INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    if (INPUT_MANAGER.IsKeyDown(ESCAPE_KEY) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_B) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_START)) {
        popRequested = true;
        INPUT_MANAGER.ReleaseKey(ESCAPE_KEY);
    }
    UpdateArray(dt);
}

void LoadState::Render() {
    RenderArray();
}

void LoadState::Pause() {}

void LoadState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}
