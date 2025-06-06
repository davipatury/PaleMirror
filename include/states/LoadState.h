#ifndef LOADSTATE_H
#define LOADSTATE_H

#include "core/Game.h"
#include "core/GameObject.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "utils/Text.h"
#include "core/Music.h"
#include "utils/Timer.h"

class LoadState : public State
{
public:
    LoadState();
    ~LoadState();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
};

#endif // LOADSTATE_H
