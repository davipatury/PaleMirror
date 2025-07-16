#include "entities/projectiles/HitAttack.h"

HitAttack::HitAttack(GameObject& associated, int damage, float duration) : Component(associated) {
    this->damage = damage;
    this->duration = duration;
    durationTimer.Restart();
}

void HitAttack::Start() {
    IsoCollider* collider = new IsoCollider(associated, {1.2, 1.2}, {-10, -45}, true);
    associated.AddComponent(collider);
}

void HitAttack::Update(float dt) {
    durationTimer.Update(dt);
    if (durationTimer.Get() >= duration) {
        associated.RequestDelete();
    }
}

void HitAttack::NotifyCollision(GameObject& other) {
    if (other.GetComponent("Zombie") != nullptr) {
        associated.RequestDelete();
    }
}

int HitAttack::GetDamage() {
    return damage;
}

void HitAttack::Render() {}

bool HitAttack::Is(std::string type) {
    return type == "HitAttack";
} 
