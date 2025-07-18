#ifndef PAUSEHUD_H
#define PAUSEHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"
#include "components/Sound.h"

class PauseHUD : public Component
{
public:
    PauseHUD(GameObject& associated);
    ~PauseHUD();

    void TogglePause();

    void Update(float dt);
    void Render();
    bool Is(std::string type);
private:
    bool paused = false;
    int selectedOption = 0;
    Sprite button, activeButton;
    TextHUD* resumeButtonText;
    TextHUD* quitButtonText;
    Sound* changeSound;
};

#endif // PAUSEHUD_H
