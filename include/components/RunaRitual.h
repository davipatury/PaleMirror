#ifndef RUNARITUAL_H
#define RUNARITUAL_H

#include <iostream>
#include <string>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/InputManager.h"
#include "hud/InventoryHUD.h"
#include "core/GameData.h"
#include "components/Interactable.h"
#include "hud/DialogueHUD.h"

class RunaRitual : public Component
{
public:
    RunaRitual(GameObject& associated);
    void Start();
    void Update (float dt);
    void Render();
    bool Is (std::string type);
private:
    Sprite velas;
};

#endif // RUNARITUAL_H
