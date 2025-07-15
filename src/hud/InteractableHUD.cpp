#include "hud/InteractableHUD.h"

#define INT_TEXT_MARGIN_X 10
#define INT_TEXT_MARGIN_Y 10

InteractableHUD* InteractableHUD::instance;

InteractableHUD::InteractableHUD(GameObject& associated) : Component(associated),
    keyE("Recursos/img/controls/key_e.png", 1, 1, true),
    keySpace("Recursos/img/controls/key_space.png", 1, 1, true),
    cbuttonA("Recursos/img/controls/cbutton_a.png", 1, 1, true),
    cbuttonX("Recursos/img/controls/cbutton_x.png", 1, 1, true)
{
    interactText = new TextHUD({0, 0}, "Recursos/font/PixelifySans-Regular.ttf", 17, TextHUD::TextStyle::BLENDED, "Pressione \"E\" para interagir", {255, 255, 255, 255});

    // Icons
    keyE.SetScale(0.5, 0.5);
    keySpace.SetScale(0.5, 0.5);
    cbuttonA.SetScale(0.5, 0.5);
    cbuttonX.SetScale(0.5, 0.5);
}

InteractableHUD::~InteractableHUD() {}

void InteractableHUD::Update(float dt) {
}

void InteractableHUD::Render() {
    if (interactableGO != nullptr) {
        Sprite key;
        if (INPUT_MANAGER.HasController()) {
            if (type == INTERACT)       key = cbuttonA;
            else if (type == USE_ITEM)  key = cbuttonX;
        } else {
            if (type == INTERACT)       key = keyE;
            else if (type == USE_ITEM)  key = keySpace;
        }

        Vec2 keyPos = interactableGO->box.TopLeft() - Camera::pos + offset;
        key.Render(keyPos.x, keyPos.y, key.GetWidth(), key.GetWidth());

        interactText->SetText(text);
        Vec2 textPos = keyPos.Add({(float) keyE.GetWidth(), (keyE.GetHeight() - interactText->GetHeight()) / 2});
        interactText->SetPos(textPos);
        interactText->Render();

        interactableGO = nullptr;
    }
}

bool InteractableHUD::Is(std::string type) {
    return type == "InteractableHUD";
}

void InteractableHUD::RecordInteractable(GameObject* go, Vec2 offset, InteractableType type, std::string text) {
    interactableGO = go;
    this->offset = offset;
    this->type = type;
    this->text = text;
}
