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
    bool won = !!GameData::playerVictory;

    backgroundMusic.Open(won ? "Recursos/audio/music/creditos.mp3" : "Recursos/audio/music/game_over.wav");
    backgroundMusic.Play();

    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, (won ? "Recursos/img/menu/creditos.png" : "Recursos/img/menu/black.png"), 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -1;
    bg->lazyRender = false;
    AddObject(bg);

    std::string endText = won ? "CREDITOS_MUITO_FODA.png" : "YOU DIED";
    text = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 80, TextHUD::SOLID, endText, {79, 2, 3, 255});
    Vec2 endTextPos = {(float) ((WINDOW_WIDTH - text->GetWidth()) * 0.5), (float) ((WINDOW_HEIGHT - text->GetHeight()) * 0.5)};
    text->SetPos(endTextPos);
    text->SetHiding(won);

    if (!won) {
        GameObject* blood = new GameObject();
        SpriteRenderer* bloodSprite = new SpriteRenderer(*blood, "Recursos/img/hud/dmg3.png", 1, 1);
        bloodSprite->SetCameraFollower(true);
        blood->AddComponent(bloodSprite);
        blood->box.z = 1;
        blood->lazyRender = false;
        AddObject(blood);
    }
}

void EndState::Start() {
    LoadAssets();
    StartArray();
    started = true;

    // Restart variables
    GameData::playerVictory = false;
    GameData::zombieFarAway = true;
    GameData::runeState = GameData::RUNA_ALAGADA;

    GameData::fusePuzzleSolved = false;
    GameData::lockPuzzleSolved = false;
    GameData::mirrorPuzzleSolved = false;
    GameData::mirrorzinhoPuzzleSolved = false;
    GameData::paintPuzzleSolved = false;
    GameData::pipePuzzleSolved = false;
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
    text->Render();
}

void EndState::Pause() {}
void EndState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
}
