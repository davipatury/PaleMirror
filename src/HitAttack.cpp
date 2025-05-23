#include "HitAttack.h"

HitAttack::HitAttack(GameObject& associated, int damage, float duration) : Component(associated) {
    this->damage = damage;
    this->duration = duration;
    durationTimer.Restart();
}

void HitAttack::Start() {
    Collider* collider = new Collider(associated);
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

    if (other.GetComponent("Character") != nullptr) {
        Character* character = (Character*)other.GetComponent("Character");
        if (character != Character::player) {
            associated.RequestDelete();
        }
    }
}

int HitAttack::GetDamage() {
    return damage;
}

void HitAttack::Render() {}

bool HitAttack::Is(std::string type) {
    return type == "HitAttack";
} 