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

    if (ESCAPE_CHECK || BACK_CHECK) {
        popRequested = true;
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
