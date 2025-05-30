#ifndef HEALTHHUD_H
#define HEALTHHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/Text.h"

class HealthHUD : public Component
{
public:
    HealthHUD(GameObject& associated);
    ~HealthHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);
private:
    Text* hpText;
};

#endif // HEALTHHUD_H
