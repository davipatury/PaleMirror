#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>
#include <string>

#include "Animation.h"
#include "Animator.h"
#include "Bullet.h"
#include "Camera.h"
#include "Collider.h"
#include "Component.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SpriteRenderer.h"
#include "Sound.h"
#include "Timer.h"

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
