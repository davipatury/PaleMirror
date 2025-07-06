#ifndef HEALTHHUD_H
#define HEALTHHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"

class HealthHUD : public Component
{
public:
    HealthHUD(GameObject& associated);
    ~HealthHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);
private:
    Sprite dmg1, dmg2, dmg3;
    int selToolbar;
    Sprite toolbar;
    Sprite toolbarselect;
};

#endif // HEALTHHUD_H
