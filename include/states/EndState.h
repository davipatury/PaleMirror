#ifndef ENDSTATE_H
#define ENDSTATE_H

#include "core/Game.h"
#include "core/GameData.h"
#include "core/GameObject.h"
#include "core/Music.h"
#include "components/SpriteRenderer.h"
#include "core/State.h"
#include "utils/Text.h"
#include "utils/TextHUD.h"

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
    TextHUD* text;
};

#endif // ENDSTATE_H
