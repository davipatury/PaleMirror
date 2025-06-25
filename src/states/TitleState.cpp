#include "states/TitleState.h"
#include "states/StageState.h"
#include "states/LoadState.h"

TitleState::TitleState(): button1("Recursos/img/menu/botao.png"),
                         button2("Recursos/img/menu/botao2.png") {
    started = false;
    quitRequested = false;
    popRequested = false;

    selectedOption = 0;

    blinkTimer.Restart();
    blinkVisible = true;

    titleMusic.Open("Recursos/audio/Title.wav");
    titleMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/menu/Home.jpeg", 1, 1);
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

    //button1.Render(38, 643, 511, 168);
    //button2.Render(38, 643, 511, 168);

    //button2.Render(622, 647, 520, 176);

    SDL_Rect highlightRect;

    if(!blinkVisible){
        button1.Open("Recursos/img/menu/botao.png");
        button2.Open("Recursos/img/menu/botao.png");
        button1.Render(48, 643, 511, 168);
        button2.Render(622, 643, 511, 168);
    }else{
        if (selectedOption == 0) {
            button1.Open("Recursos/img/menu/botao2.png");
            button2.Open("Recursos/img/menu/botao.png");
            button1.Render(48, 643, 520, 176);
            button2.Render(622, 643, 511, 168);
        }else{
            button1.Open("Recursos/img/menu/botao.png");
            button2.Open("Recursos/img/menu/botao2.png");
            button1.Render(48, 643, 511, 168);
            button2.Render(622, 643, 520, 176);
        }
    }
    static GameObject* textGO     = nullptr;
    static Text* promptText = nullptr;
    textGO = new GameObject();
    textGO->box = Rect(0, 0, 0, 0);
    const char* fontFile = "Recursos/font/PixelifySans-Regular.ttf";
    int fontSize = 70;
    Text::TextStyle style = Text::BLENDED;
    std::string msg = "Novo";
    SDL_Color color = { 0, 0, 0, 0 };
    float flash = 0.0f;
    promptText = new Text(*textGO, fontFile, fontSize, style, msg, color, flash);
    textGO->AddComponent(promptText);
    textGO->box.x = 200;
    textGO->box.y = 685;
    promptText->Render();


    static GameObject* textGO2     = nullptr;
    static Text* promptText2 = nullptr;
    textGO2 = new GameObject();
    textGO2->box = Rect(0, 0, 0, 0);
    std::string msg2 = "Carregar";
    promptText2 = new Text(*textGO2, fontFile, fontSize, style, msg2, color, flash);
    textGO2->AddComponent(promptText);
    textGO2->box.x = 730;
    textGO2->box.y = 685;
    promptText2->Render();



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
