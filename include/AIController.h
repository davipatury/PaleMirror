#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <iostream>
#include <string>

#include "Character.h"
#include "Component.h"
#include "Timer.h"
#include "Vec2.h"

class AIController : public Component
{
public:
    AIController(GameObject& associated);
    ~AIController();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    static int npcCounter;
private:
    enum AIState { MOVING, RESTING };
    AIState state;
    Timer restTimer;
    Vec2 destination;
};

#endif // AICONTROLLER_H
