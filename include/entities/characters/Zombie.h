#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <iostream>
#include <string>

#include "utils/Animation.h"
#include "components/Animator.h"
#include "components/Camera.h"
#include "physics/Collider.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "components/SpriteRenderer.h"
#include "components/Sound.h"
#include "utils/Timer.h"
#include <unordered_set>

class Zombie : public Component
{
public:
    Zombie(GameObject& associated);
    ~Zombie() override;

    void Damage(int damage);
    void Start();
    void Update(float dt);
    void Render();

    void NotifyCollision(GameObject& other);

    bool Is(std::string type);

    static int zombieCounter;
private:
    int hitpoints;
    Sound deathSound;
    Sound hitSound;
    Sound monsterSound;

    Timer deathTimer;
    Timer hitTimer;
    Timer monsterSoundTimer;
    bool hit;
    bool walkingLeft;
    float chaseRadius = 500.f;
    std::vector<Vec2> path;
    int pathIndex = 0;
    const float speed = 100.f;
    const float searchStep = 64.f;

    Rect isoB;

    std::vector<Vec2> computePath(const Vec2& target);
    bool checkCollision(float x, float y);
};

#endif // ZOMBIE_H
