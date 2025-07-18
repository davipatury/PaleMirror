#include "hud/PauseHUD.h"

#include "core/Game.h"

PauseHUD::PauseHUD(GameObject& associated) : Component(associated),
    button("Recursos/img/menu/butao-sombreado.png", 1, 1, true),
    activeButton("Recursos/img/menu/butao-mais-destacado.png", 1, 1, true)
{
    resumeButtonText = new TextHUD({225, 700}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Continuar", {0, 0, 0, 0});
    quitButtonText = new TextHUD({704, 700}, "Recursos/font/PixelifySans-Regular.ttf", 75, TextHUD::BLENDED, "Sair", {0, 0, 0, 0});
    changeSound = new Sound("Recursos/audio/sounds/menu/click_menu.wav");
}

PauseHUD::~PauseHUD() {}

void PauseHUD::Update(float dt) {
    if (ESCAPE_CHECK) TogglePause();

    if (paused) {
        if (UP_CHECK && selectedOption == 1) {
            changeSound->Play();
            selectedOption = 0;
        }
        if (DOWN_CHECK && selectedOption == 0) {
            changeSound->Play();
            selectedOption = 1;
        }
        if (CONFIRM_CHECK) {
            changeSound->Play();
            if (selectedOption == 0) TogglePause();
            else CURRENT_STATE.RequestPop();
        }
        if (BACK_CHECK) TogglePause();
    }
}

void PauseHUD::Render() {
    if (!paused) return;

    // Semi transparent black background
    SDL_Rect screenRect = WINDOW_RECT;
    SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    int buttonX = (WINDOW_WIDTH - button.GetWidth()) * 0.5;
    int buttonMargin = 20;
    int topButtonY = (WINDOW_HEIGHT * 0.5) - button.GetHeight() - buttonMargin;
    int bottomButtonY = (WINDOW_HEIGHT * 0.5) + buttonMargin;
    int activeMarginX = -12;
    int activeMarginY = -8;
    if (selectedOption == 0) {
        // Resume
        activeButton.Render(buttonX + activeMarginX, topButtonY + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
        // Quit
        button.Render(buttonX, bottomButtonY, button.GetWidth(), button.GetHeight());
    } else {
        // Resume
        button.Render(buttonX, topButtonY, button.GetWidth(), button.GetHeight());
        // Quit
        activeButton.Render(buttonX + activeMarginX, bottomButtonY + activeMarginY, activeButton.GetWidth(), activeButton.GetHeight());
    }

    resumeButtonText->SetPos({(float) ((WINDOW_WIDTH - resumeButtonText->GetWidth()) * 0.5), (float) (topButtonY + 60)});
    resumeButtonText->Render();
    quitButtonText->SetPos({(float) ((WINDOW_WIDTH - quitButtonText->GetWidth()) * 0.5), (float) (bottomButtonY + 60)});
    quitButtonText->Render();
}

void PauseHUD::TogglePause() {
    paused = !paused;
    if (paused) {
        associated.pauseOnOpenUI = false;
        CURRENT_STATE.openUI = true;
    } else {
        associated.pauseOnOpenUI = true;
        CURRENT_STATE.openUI = false;
    }
}

bool PauseHUD::Is(std::string type) {
    return type == "PauseHUD";
}
