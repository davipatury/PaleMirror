#ifndef FLASHLIGHTHUD_H
#define FLASHLIGHTHUD_H

#include <iostream>
#include <string>
#include <sstream>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "hud/InventoryHUD.h"
#include "components/LightEmitter.h"
#include "components/Sound.h"

#define FLASHLIGHT FlashlightHUD::instance

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

    static FlashlightHUD* instance;
private:
    Vec2 origin;
    Sprite backlight;
    SDL_Texture* texture;
    SDL_Texture* supportTexture;
    bool isDark;
    bool flashlightOn;
    float angle;

    Sound* toggleSound;
};

#endif // FLASHLIGHTHUD_H
