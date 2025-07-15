#include "hud/InteractableHUD.h"

#define INT_TEXT_MARGIN_X 10
#define INT_TEXT_MARGIN_Y 10

bool InteractableHUD::hasInteractable;

InteractableHUD::InteractableHUD(GameObject& associated) : Component(associated) {
    text = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 30, TextHUD::TextStyle::BLENDED, "Pressione \"E\" para interagir", {255, 255, 255, 255});
    text->SetPos({1200 - INT_TEXT_MARGIN_X - text->GetWidth(), 900 - INT_TEXT_MARGIN_Y - text->GetHeight()});
    hasInteractable = false;
}

InteractableHUD::~InteractableHUD() {}

void InteractableHUD::Update(float dt) {

}

void InteractableHUD::Render() {
    if (hasInteractable) {
        text->Render();
        hasInteractable = false;
    }
}

bool InteractableHUD::Is(std::string type) {
    return type == "InteractableHUD";
}

void InteractableHUD::RecordInteractable() {
    hasInteractable = true;
}
