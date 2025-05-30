#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>
#include <string>

#include "utils/Animation.h"
#include "components/Animator.h"
#include "entities/projectiles/Bullet.h"
#include "components/Camera.h"
#include "physics/Collider.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "components/SpriteRenderer.h"
#include "components/Sound.h"
#include "utils/Timer.h"

class Zombie : public Component
{
public:
    Zombie(GameObject& associated);

    void Damage(int damage);
    void Start();
    void Update(float dt);
    void Render();

    void NotifyCollision(GameObject& other);

    bool Is(std::string type);

    static int zombieCounter;
private:
    int hitpoints;
    Sound* deathSound;
    Sound* hitSound;
    Timer deathTimer;
    Timer hitTimer;
    bool hit;
    bool walkingLeft;
};

#endif // ZOMBIE_H
