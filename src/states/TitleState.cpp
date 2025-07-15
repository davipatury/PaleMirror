#include "states/TitleState.h"
#include "states/StageState.h"
#include "states/LoadState.h"

TitleState::TitleState(): button("Recursos/img/menu/butao-sombreado.png"),
                         activeButton("Recursos/img/menu/butao-mais-destacado.png") {
    started = false;
    quitRequested = false;
    popRequested = false;

    selectedOption = 0;
}

TitleState::~TitleState(){
    objectArray.clear();
}

void TitleState::LoadAssets() {
    titleMusic.Open("Recursos/audio/Title.mp3");
    titleMusic.Play();

    bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/menu/Home.jpeg", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    startButtonText = new TextHUD({225, 700}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Novo", {0, 0, 0, 0});
    loadButtonText = new TextHUD({704, 700}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Carregar", {0, 0, 0, 0});
}

void TitleState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::Update(float dt) {
    // Quit requested
    if (ESCAPE_CHECK || INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    if (tutorial) {
        if (CONFIRM_CHECK) Game::GetInstance().Push(new StageState());
    } else {
        bool leftPressed = INPUT_MANAGER.KeyPress(LEFT_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_a) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
        bool rightPressed = INPUT_MANAGER.KeyPress(RIGHT_ARROW_KEY) || INPUT_MANAGER.KeyPress(SDLK_d) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
        if (INPUT_MANAGER.HasController()) {
            Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK);
            leftPressed = leftPressed || leftAxis.x < 0;
            rightPressed = rightPressed || leftAxis.x > 0;
        }

        if (leftPressed) {
            selectedOption = 0;
        }
        if (rightPressed) {
            selectedOption = 1;
        }

        if (CONFIRM_CHECK) {
            if (selectedOption == 0) {
                SpriteRenderer* sr = (SpriteRenderer*) bg->GetComponent("SpriteRenderer");
                if (INPUT_MANAGER.HasController()) sr->Open("Recursos/img/menu/controles_controller.png");
                else sr->Open("Recursos/img/menu/controles_keyboard.png");
                tutorial = true;
            } else Game::GetInstance().Push(new LoadState());
        }
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
    RenderArray();

    if (!tutorial) {
        int margin = 100;
        int activeMarginX = -12;
        int activeMarginY = -8;
        if (selectedOption == 0) {
            // Novo
            activeButton.Render(margin + activeMarginX, 646 + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
            // Carregar
            button.Render(WINDOW_WIDTH - margin - button.GetWidth(), 646, button.GetWidth(), button.GetHeight());
        } else {
            // Novo
            button.Render(margin, 646, button.GetWidth(), button.GetHeight());
            // Carregar
            activeButton.Render(WINDOW_WIDTH - margin - button.GetWidth() + activeMarginX, 646 + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
        }

        startButtonText->Render();
        loadButtonText->SetPos({WINDOW_WIDTH - margin - button.GetWidth() + 25, 700});
        loadButtonText->Render();
    }
}

void TitleState::Pause() {
    titleMusic.Stop();
}

void TitleState::Resume() {
    tutorial = false;
    SpriteRenderer* sr = (SpriteRenderer*) bg->GetComponent("SpriteRenderer");
    sr->Open("Recursos/img/menu/Home.jpeg");

    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;
    titleMusic.Play();
}
