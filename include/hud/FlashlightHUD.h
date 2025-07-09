#ifndef FLASHLIGHTHUD_H
#define FLASHLIGHTHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"

class FlashlightHUD : public Component
{
public:
    FlashlightHUD(GameObject& associated);
    ~FlashlightHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    bool IsDark();
    bool IsFlashlightOn();

    void SetDark(bool b);
    void SetFlashlight(bool b);

    void ToggleDarkMode();
    void ToggleFlashlight();
private:
    Sprite backlight;
    SDL_Texture* texture;
    bool isDark;
    bool flashlightOn;
    float angle;
};

#endif // FLASHLIGHTHUD_H
