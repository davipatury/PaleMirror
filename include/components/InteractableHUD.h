#ifndef INTERACTABLEHUD_H
#define INTERACTABLEHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"

class InteractableHUD : public Component
{
public:
    InteractableHUD(GameObject& associated);
    ~InteractableHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    static void RecordInteractable();
private:
    TextHUD* text;
    static bool hasInteractable;
};

#endif // INTERACTABLEHUD_H
