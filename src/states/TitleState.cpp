#include "states/TitleState.h"
#include "states/StageState.h"
#include "states/LoadState.h"

#define MARGIN 100

TitleState::TitleState() :
    button("Recursos/img/menu/butao-sombreado.png"),
    activeButton("Recursos/img/menu/butao-mais-destacado.png"),
    menuBg("Recursos/img/menu/menu.png", 5, 1, true),
    menuControls("Recursos/img/menu/controles.png", 2, 1, true)
{
    started = false;
    quitRequested = false;
    popRequested = false;

    selectedOption = 0;

    bgBlinkTimer = Timer();
}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets() {
    titleMusic.Open("Recursos/audio/Title.mp3");
    titleMusic.Play();

    changeSound = new Sound("Recursos/audio/sounds/menu/click_menu.wav");
    playSound = new Sound("Recursos/audio/sounds/menu/click_start.wav");

    GameObject* logo = new GameObject();
    SpriteRenderer* logoSprite = new SpriteRenderer((*logo), "Recursos/img/menu/logo.png");
    logoSprite->SetCameraFollower(true);
    logo->AddComponent(logoSprite);
    logo->box.x = 35;
    logo->box.y = 35;
    logo->box.z = -1;
    AddObject(logo);

    float textY = 700;
    startButtonText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Jogar", {0, 0, 0, 0});
    loadButtonText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Sair", {0, 0, 0, 0});

    float startX = MARGIN + (button.GetWidth() - startButtonText->GetWidth()) * 0.5;
    float loadX = WINDOW_WIDTH - MARGIN - (button.GetWidth() + loadButtonText->GetWidth()) * 0.5;
    startButtonText->SetPos({startX, textY});
    loadButtonText->SetPos({loadX, textY});
}

void TitleState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::Update(float dt) {
    // Quit requested
    if (INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    bgBlinkTimer.Update(dt);

    if (tutorial) {
        if (INPUT_MANAGER.HasController()) menuControls.SetFrame(0);
        else menuControls.SetFrame(1);

        if (CONFIRM_CHECK) {
            playSound->Play();
            Game::GetInstance().Push(new StageState());
        }
        if (ESCAPE_CHECK || BACK_CHECK) {
            changeSound->Play();
            tutorial = false;
        }
    } else {
        // Piscar
        if (blinking) {
            if (bgBlinkTimer.Get() > 0.7) {
                blinking = false;
                bgBlinkTimer.Restart();
            }
        } else {
            menuBg.SetFrame(1);
            if (bgBlinkTimer.Get() > 4) {
                blinking = true;
                int randomFrame = rand() % 4;
                if (randomFrame > 0) randomFrame++;
                menuBg.SetFrame(randomFrame);
                bgBlinkTimer.Restart();
            }
        }

        if (LEFT_CHECK && selectedOption == 1) { selectedOption = 0; changeSound->Play(); }
        if (RIGHT_CHECK && selectedOption == 0) { selectedOption = 1; changeSound->Play(); }
        if (CONFIRM_CHECK) {
            changeSound->Play();
            if (selectedOption == 0) tutorial = true;
            else quitRequested = true;
        }
        if (ESCAPE_CHECK) quitRequested = true;
    }

    // Fullscreen
    if (INPUT_MANAGER.KeyPress(SDLK_F11)) {
        Uint32 flags = SDL_GetWindowFlags(GAME_WINDOW);
        bool isFullscreen = flags & SDL_WINDOW_FULLSCREEN;
        SDL_SetWindowFullscreen(GAME_WINDOW, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    if (tutorial) menuControls.Render(0, 0, menuControls.GetWidth(), menuControls.GetHeight());
    else          menuBg.Render(0, 0, menuBg.GetWidth(), menuBg.GetHeight());

    RenderArray();

    if (!tutorial) {
        int activeMarginX = -12;
        int activeMarginY = -8;
        if (selectedOption == 0) {
            // Novo
            activeButton.Render(MARGIN + activeMarginX, 646 + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
            // Carregar
            button.Render(WINDOW_WIDTH - MARGIN - button.GetWidth(), 646, button.GetWidth(), button.GetHeight());
        } else {
            // Novo
            button.Render(MARGIN, 646, button.GetWidth(), button.GetHeight());
            // Carregar
            activeButton.Render(WINDOW_WIDTH - MARGIN - button.GetWidth() + activeMarginX, 646 + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
        }

        startButtonText->Render();
        loadButtonText->Render();
    }
}

void TitleState::Pause() {
    titleMusic.Stop();
}

void TitleState::Resume() {
    tutorial = false;

    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
    titleMusic.Play();
}
