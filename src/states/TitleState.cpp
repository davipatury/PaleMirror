#include "states/TitleState.h"
#include "states/StageState.h"
#include "states/LoadState.h"

TitleState::TitleState(): button("Recursos/img/menu/botao.png"),
                         activeButton("Recursos/img/menu/botao_ativo.png") {
    started = false;
    quitRequested = false;
    popRequested = false;

    selectedOption = 0;
}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets() {
    titleMusic.Open("Recursos/audio/Title.wav");
    titleMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/menu/Home.jpeg", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    startButtonText = new TextHUD({225, 680}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Novo", {0, 0, 0, 0});
    loadButtonText = new TextHUD({720, 680}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Carregar", {0, 0, 0, 0});
}

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
        selectedOption = 0;
    }

    if (INPUT_MANAGER.KeyPress(RIGHT_ARROW_KEY) or INPUT_MANAGER.KeyPress(SDLK_d)) {
        selectedOption = 1;
    }

    if (INPUT_MANAGER.KeyPress(SDLK_RETURN) or INPUT_MANAGER.IsKeyDown(SDLK_SPACE)) {
        if (selectedOption == 0) Game::GetInstance().Push(new StageState());
        else Game::GetInstance().Push(new LoadState());
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    RenderArray();

    int margin = 60;
    if (selectedOption == 0) {
        // Novo
        activeButton.Render(margin, 643, activeButton.GetWidth(), activeButton.GetHeight());
        // Carregar
        button.Render(1200 - margin - button.GetWidth(), 643, button.GetWidth(), button.GetHeight());
    } else {
        // Novo
        button.Render(margin, 643, button.GetWidth(), button.GetHeight());
        // Carregar
        activeButton.Render(1200 - margin - button.GetWidth(), 643, activeButton.GetWidth(), activeButton.GetHeight());
    }

    startButtonText->Render();
    loadButtonText->Render();
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
