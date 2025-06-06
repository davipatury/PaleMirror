#ifndef GUN_H
#define GUN_H

#include <iostream>
#include <string>
#include <memory>
#include <math.h>

#include "components/Animator.h"
#include "entities/projectiles/Bullet.h"
#include "core/GameObject.h"
#include "components/Sound.h"
#include "components/SpriteRenderer.h"
#include "utils/Timer.h"

class Gun : public Component
{
public:
    Gun(GameObject& associated, std::weak_ptr<GameObject> character);
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void UpdateBox();
    void Shoot(Vec2 target);
private:
    Sound shotSound;
    Sound reloadSound;
    int cooldown;
    Timer cdTimer;
    std::weak_ptr<GameObject> character;
    float angle;
};

#endif // GUN_H
