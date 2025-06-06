#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "core/Game.h"
#include "core/GameObject.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "utils/Text.h"
#include "core/Music.h"

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
    int selectedOption; // 0 = Novo, 1 = Carregar
};

#endif // TITLESTATE_H
