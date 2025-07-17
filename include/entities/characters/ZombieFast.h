#ifndef ZOMBIEFAST_H
#define ZOMBIEFAST_H

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

class ZombieFast : public Component
{
public:
    ZombieFast(GameObject& associated);
    ~ZombieFast() override;

    void Damage(int damage);
    void Start();
    void Update(float dt);
    void Render();

    void NotifyCollision(GameObject& other);

    bool Is(std::string type);
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
    float chaseRadius = 600.f;
    std::vector<Vec2> path;
    int pathIndex = 0;
    const float speed = 190.f;
    const float searchStep = 64.f;

    std::string currentSprite;

    Rect isoB;

    std::vector<Vec2> computePath(const Vec2& target);
    bool checkCollision(float x, float y);
};

#endif // ZOMBIEFAST_H
