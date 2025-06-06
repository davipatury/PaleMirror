#include "states/TitleState.h"

#include "states/StageState.h"

TitleState::TitleState() {
    started = false;
    quitRequested = false;
    popRequested = false;
    selectedOption = 0; // 0 = Novo, 1 = Carregar

    titleMusic.Open("Recursos/audio/Title.wav");
    titleMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/Home.jpeg", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    // Retângulo Novo
    GameObject* novoRect = new GameObject();
    novoRect->box.x = 400;
    novoRect->box.y = 400;
    novoRect->box.w = 200;
    novoRect->box.h = 50;
    novoRect->box.z = 0;
    AddObject(novoRect);

    // Texto Novo
    GameObject* novoText = new GameObject();
    Text* novoTextCmp = new Text((*novoText), "Recursos/font/neodgm.ttf", 40, Text::TextStyle::SOLID, "NOVO", {255, 255, 255, 255});
    novoText->AddComponent(novoTextCmp);
    novoText->box.x = 450;
    novoText->box.y = 410;
    novoText->box.z = 1;
    AddObject(novoText);

    // Retângulo Carregar
    GameObject* carregarRect = new GameObject();
    carregarRect->box.x = 600;
    carregarRect->box.y = 400;
    carregarRect->box.w = 200;
    carregarRect->box.h = 50;
    carregarRect->box.z = 0;
    AddObject(carregarRect);

    // Texto Carregar
    GameObject* carregarText = new GameObject();
    Text* carregarTextCmp = new Text((*carregarText), "Recursos/font/neodgm.ttf", 40, Text::TextStyle::SOLID, "CARREGAR", {255, 255, 255, 255});
    carregarText->AddComponent(carregarTextCmp);
    carregarText->box.x = 610;
    carregarText->box.y = 410;
    carregarText->box.z = 1;
    AddObject(carregarText);
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

    // Navegação entre opções
    if (InputManager::GetInstance().KeyPress(SDLK_RIGHT)) {
        selectedOption = 1;
    }
    if (InputManager::GetInstance().KeyPress(SDLK_LEFT)) {
        selectedOption = 0;
    }

    // Seleção da opção
    if (InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) {
        if (selectedOption == 0) {
            Game::GetInstance().Push(new StageState());
        } else {
            // TODO: Implementar carregamento
        }
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    RenderArray();

    // Desenha o retângulo de seleção
    SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 255, 255, 255);
    SDL_Rect rect;
    if (selectedOption == 0) {
        rect = {400, 400, 200, 50};
    } else {
        rect = {600, 400, 200, 50};
    }
    SDL_RenderDrawRect(Game::GetInstance().GetRenderer(), &rect);
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
