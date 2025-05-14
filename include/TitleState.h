#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "Game.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "State.h"
#include "Text.h"

class TitleState : public State
{
public:
    TitleState();
    ~TitleState();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
};

#endif // TITLESTATE_H
