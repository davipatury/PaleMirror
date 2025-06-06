#ifndef COLLIDER_H
#define COLLIDER_H

#include <iostream>
#include <string>

#include "core/GameObject.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class Collider : public Component
{
public:
    Collider(GameObject& associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);

    Rect box;
private:
    Vec2 scale;
    Vec2 offset;
};

#endif // COLLIDER_H
