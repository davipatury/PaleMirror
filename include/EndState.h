#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "Game.h"
#include "GameData.h"
#include "GameObject.h"
#include "Music.h"
#include "SpriteRenderer.h"
#include "State.h"
#include "Text.h"

class EndState : public State
{
public:
    EndState();
    ~EndState();

    void LoadAssets();
    void Update(float dt);
    void Render();
    void Start();
    void Pause();
    void Resume();
private:
    Music backgroundMusic;
};

#endif // ENDSTATE_H
