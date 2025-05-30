#include "entities/projectiles/Bullet.h"

Bullet::Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, bool targetsPlayer) : Component(associated) {
    this->damage = damage;
    this->distanceLeft = maxDistance;
    this->speed.x = speed;
    this->speed = this->speed.Rotate(angle);
    this->targetsPlayer = targetsPlayer;

    SpriteRenderer* spriteRdr = new SpriteRenderer(associated, "Recursos/img/Bullet.png");
    associated.AddComponent(spriteRdr);
}

void Bullet::Start() {
    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);
}

void Bullet::Update(float dt) {
    Vec2 distance = speed.MulScalar(dt);
    associated.box = associated.box.Add(distance);
    distanceLeft -= distance.Magnitude();
    if (distanceLeft <= 0) {
        associated.RequestDelete();
    }
}

void Bullet::NotifyCollision(GameObject &other) {
    if (other.GetComponent("Zombie") != nullptr) {
        associated.RequestDelete();
    }

    if (other.GetComponent("Character") != nullptr) {
        if (targetsPlayer && other.GetComponent("Character") == Character::player) {
            associated.RequestDelete();
        }
        if (!targetsPlayer && other.GetComponent("Character") != Character::player) {
            associated.RequestDelete();
        }
    }
}

int Bullet::GetDamage() {
    return damage;
}

void Bullet::Render() {}

bool Bullet::Is(std::string type) {
    return type == "Bullet";
}
