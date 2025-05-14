#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <iostream>
#include <string>

#include "Camera.h"
#include "Character.h"
#include "Component.h"
#include "InputManager.h"

class PlayerController : public Component
{
public:
    PlayerController(GameObject& associated);
    void Start();
    void Update (float dt);
    void Render();
    bool Is (std::string type);
};

#endif // PLAYERCONTROLLER_H
