#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "core/Game.h"
#include "core/GameObject.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "utils/Text.h"
#include "core/Music.h"
#include "utils/Timer.h"

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

    private:
    Music titleMusic;
    int selectedOption;

    Timer blinkTimer;
    bool  blinkVisible;
    static constexpr float BLINK_INTERVAL = 0.5f;
};

#endif // TITLESTATE_H
