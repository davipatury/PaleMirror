#ifndef ISOCOLLIDER_H
#define ISOCOLLIDER_H

#include <iostream>
#include <string>
#include <vector>

#include "core/GameObject.h"
#include "math/Line.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class IsoCollider : public Component
{
public:
    IsoCollider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0}, bool passable = false, bool blockLight = false);

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);
    Vec2 GetScale();
    Vec2 GetOffset();

    void UpdateBox();

    Rect prevBox;
    Rect box;
    bool passable;
    bool blockLight;

    Rect MakeIsoBoxForPoint(float x, float y);
private:
    Vec2 scale;
    Vec2 offset;
};

#endif // ISOCOLLIDER_H
