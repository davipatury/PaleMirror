#include "states/EndState.h"

EndState::EndState() {
    started = false;
    quitRequested = false;
    popRequested = false;
}

EndState::~EndState(){
    objectArray.clear();
}

void EndState::LoadAssets() {
    backgroundMusic.Open(GameData::playerVictory ? "Recursos/audio/endStateWin.ogg" : "Recursos/audio/endStateLose.ogg");
    backgroundMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, GameData::playerVictory ? "Recursos/img/Win.png" : "Recursos/img/black.png", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    GameObject* text = new GameObject();
    text->AddComponent(new Text(*text, "Recursos/font/PixelifySans-Regular.ttf", 30, Text::TextStyle::SOLID, GameData::playerVictory ? "Parabens! Voce venceu!" : "Voce perdeu :(", {255, 255, 255, 255}));
    text->box.x = (WINDOW_WIDTH - text->box.w) * 0.5;
    text->box.y = (WINDOW_HEIGHT - text->box.h) * 0.5;
    text->box.z = 0;
    AddObject(text);

    if (!GameData::playerVictory) {
        GameObject* blood = new GameObject();
        SpriteRenderer* bloodSprite = new SpriteRenderer(*blood, "Recursos/img/hud/dmg3.png", 1, 1);
        bloodSprite->SetCameraFollower(true);
        blood->AddComponent(bloodSprite);
        blood->box.z = 1;
        AddObject(blood);
    }
}

void EndState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void EndState::Update(float dt) {
    // Quit requested
    if (ESCAPE_CHECK || INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    // Start
    if (CONFIRM_CHECK) {
        popRequested = true;
    }

    UpdateArray(dt);
}

void EndState::Render() {
    RenderArray();
}

void EndState::Pause() {}
void EndState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}
