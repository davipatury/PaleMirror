#ifndef DEBUGHUD_H
#define DEBUGHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"

class DebugHUD : public Component
{
public:
    enum DebugMode { DEBUG_DISABLE, DEBUG_POS, DEBUG_SCALE, DEBUG_ISO_POS, DEBUG_ISO_SCALE };

    DebugHUD(GameObject& associated);
    ~DebugHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);
private:
    TextHUD* text;
    TextHUD* supportText;
    std::string support = "";
    DebugMode debugMode = DEBUG_DISABLE;
    GameObject* selectedObject;
};

#endif // DEBUGHUD_H
