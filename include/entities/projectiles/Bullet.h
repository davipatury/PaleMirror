#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include <string>

#include "entities/characters/Character.h"
#include "physics/Collider.h"
#include "core/Component.h"
#include "components/SpriteRenderer.h"
#include "math/Vec2.h"

class Bullet : public Component
{
public:
    Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer);
    void Update(float dt);
    void Start();
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other);
    int GetDamage();

    bool targetsPlayer;
private:
    Vec2 speed;
    float distanceLeft;
    int damage;
};

#endif // BULLET_H
