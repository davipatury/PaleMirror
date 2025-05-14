#ifndef BULLET_H
#define BULLET_H

#include <iostream>
#include <string>

#include "Character.h"
#include "Collider.h"
#include "Component.h"
#include "SpriteRenderer.h"
#include "Vec2.h"

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
