#ifndef CLASSROOMSTATE_H
#define CLASSROOMSTATE_H

#include "core/Game.h"
#include "core/GameObject.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "utils/Text.h"

class ClassRoomState : public State
{
public:
    ClassRoomState();
    ~ClassRoomState();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
    float posReturnX;
    float posReturnY;
};

#endif // CLASSROOMSTATE_H
