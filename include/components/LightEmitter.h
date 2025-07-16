#ifndef LIGHTEMITTER_H
#define LIGHTEMITTER_H

#include <iostream>
#include <string>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/InputManager.h"
#include "hud/InventoryHUD.h"

class LightEmitter : public Component
{
public:
    class LightPoint {
    public:
        LightPoint(Vec2 o = {0, 0}, Vec2 s = {0.5, 0.5}, bool e = true, const char* f = "Recursos/img/lighting/backlight_inv.png");
        Vec2 scale;
        Vec2 offset;
        bool enabled;
        Sprite sprite;
    };

    LightEmitter(GameObject& associated, std::vector<LightPoint> lightPoints);
    void Start();
    void Update (float dt);
    void Render();
    bool Is(std::string type);

    bool IsEnabled(int pointIndex);
    bool HasEnabledPoint();

    void SetEnabled(int pointIndex, bool enabled);
    void SetEnabledAll(bool enabled);

    void SetScale(int pointIndex, float scaleX, float scaleY);
    void SetScaleAll(float scaleX, float scaleY);

    void SetOffset(int pointIndex, Vec2 offset);
    void SetOffsetAll(Vec2 offset);

    Vec2 GetPos(int pointIndex, bool minusCamera = true);
    Vec2 GetOffset(int pointIndex);
    Vec2 GetScale(int pointIndex);
    Sprite GetSprite(int pointIndex);

    std::vector<LightPoint> lightPoints;
};

#endif // LIGHTEMITTER_H
