#ifndef HITATTACK_H
#define HITATTACK_H

#include <iostream>
#include <string>

#include "Character.h"
#include "Collider.h"
#include "Component.h"
#include "Timer.h"

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