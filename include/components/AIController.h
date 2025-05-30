#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include <iostream>
#include <string>

#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/Timer.h"
#include "math/Vec2.h"

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
