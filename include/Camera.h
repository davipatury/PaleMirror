#ifndef CAMERA_H
#define CAMERA_H

#include "GameObject.h"
#include "InputManager.h"
#include "Vec2.h"

class Camera
{
public:
    static void Follow(GameObject* newFocus);
    static void Unfollow();
    static void Update(float dt);

    static Vec2 pos;
    static Vec2 speed;
private:
    static GameObject* focus;
};

#endif // CAMERA_H
