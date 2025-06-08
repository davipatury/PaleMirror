#ifndef CAMERA_H
#define CAMERA_H

#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "math/Rect.h"
#include "math/Vec2.h"

class Camera
{
public:
    static void Follow(GameObject* newFocus);
    static void Unfollow();
    static void Update(float dt);

    static Rect PosRect();

    static Vec2 pos;
    static Vec2 speed;
private:
    static GameObject* focus;
};

#endif // CAMERA_H
