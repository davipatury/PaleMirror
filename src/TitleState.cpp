#include "TitleState.h"

#include "StageState.h"

TitleState::TitleState() {
    started = false;
    quitRequested = false;
    popRequested = false;

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/Title.png", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    GameObject* text = new GameObject();
    Text* textCmp = new Text((*text), "Recursos/font/neodgm.ttf", 40, Text::TextStyle::SOLID, "Pressione espaco para continuar", {255, 255, 255, 255}, 0.5);
    text->AddComponent(textCmp);
    text->box.x = 600 - text->box.w * 0.5;
    text->box.y = 450 - text->box.h * 0.5;
    text->box.z = 0;
    AddObject(text);
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
    if (InputManager::GetInstance().IsKeyDown(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested()) {
        quitRequested = true;
    }

    // Start
    if (InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) {
        Game::GetInstance().Push(new StageState());
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    RenderArray();
}

void TitleState::Pause() {}
void TitleState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}
