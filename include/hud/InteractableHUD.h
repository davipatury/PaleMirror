#ifndef INTERACTABLEHUD_H
#define INTERACTABLEHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"

#define INTERACTABLE InteractableHUD::instance

class InteractableHUD : public Component
{
public:
    InteractableHUD(GameObject& associated);
    ~InteractableHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    enum InteractableType { INTERACT, USE_ITEM };
    void RecordInteractable(GameObject* go, Vec2 offset, InteractableType type, std::string text);

    static InteractableHUD* instance;
private:
    Sprite keyE, keySpace, cbuttonA, cbuttonX;
    TextHUD* interactText;

    GameObject* interactableGO = nullptr;
    Vec2 offset = {0, 0};
    InteractableType type = INTERACT;
    std::string text = "";
};

#endif // INTERACTABLEHUD_H
