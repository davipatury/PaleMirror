#ifndef HEALTHHUD_H
#define HEALTHHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "Camera.h"
#include "Character.h"
#include "Component.h"
#include "Text.h"

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
