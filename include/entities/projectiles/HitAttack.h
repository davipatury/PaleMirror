#ifndef HITATTACK_H
#define HITATTACK_H

#include <iostream>
#include <string>

#include "entities/characters/Character.h"
#include "physics/Collider.h"
#include "core/Component.h"
#include "utils/Timer.h"

class HitAttack : public Component
{
public:
    HitAttack(GameObject& associated, int damage, float duration);
    void Update(float dt);
    void Start();
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other);
    int GetDamage();

private:
    Timer durationTimer;
    int damage;
    float duration;
};

#endif // HITATTACK_H 