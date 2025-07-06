#include "states/EndState.h"

EndState::EndState() {
    started = false;
    quitRequested = false;
    popRequested = false;

    backgroundMusic.Open(GameData::playerVictory ? "Recursos/audio/endStateWin.ogg" : "Recursos/audio/endStateLose.ogg");
    backgroundMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), GameData::playerVictory ? "Recursos/img/Win.png" : "Recursos/img/Lose.png", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    GameObject* text = new GameObject();
    Text* textCmp = new Text((*text), "Recursos/font/neodgm.ttf", 30, Text::TextStyle::SOLID, GameData::playerVictory ? "Parabens! Voce venceu!" : "Voce perdeu :(", {255, 255, 255, 255});
    text->AddComponent(textCmp);
    text->box.x = 600 - text->box.w * 0.5;
    text->box.y = 450 - text->box.h * 0.5;
    text->box.z = 0;
    AddObject(text);
}

EndState::~EndState(){
    objectArray.clear();
}

void EndState::LoadAssets() {}

void EndState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void EndState::Update(float dt) {
    // Quit requested
    if (INPUT_MANAGER.IsKeyDown(ESCAPE_KEY) || INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    // Start
    if (INPUT_MANAGER.IsKeyDown(SDLK_SPACE)) {
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
