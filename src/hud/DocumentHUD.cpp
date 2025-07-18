#include "hud/DocumentHUD.h"
#include "utils/InputManager.h"
#include "core/State.h"
#include "core/Game.h"

#define DOCUMENT_BG_OFFSET_X 233
#define DOCUMENT_BG_OFFSET_Y 65

DocumentHUD::DocumentHUD(GameObject& associated, const std::string& imagePath)
    : Component(associated){
    bg.Open(imagePath.c_str());
}

DocumentHUD::~DocumentHUD() {}

void DocumentHUD::Start() {
    CURRENT_STATE.openUI = true;
}

void DocumentHUD::Update(float dt) {
    if (ESCAPE_CHECK) {
        associated.RequestDelete();
    }
}

void DocumentHUD::Render() {
    SDL_Rect screenRect = WINDOW_RECT;
    SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    bg.Render(DOCUMENT_BG_OFFSET_X, DOCUMENT_BG_OFFSET_Y, bg.GetWidth(), bg.GetHeight());

}

bool DocumentHUD::Is(std::string type) {
    return type == "DocumentHUD";
}
