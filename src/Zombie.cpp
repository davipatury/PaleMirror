#include "Zombie.h"

int Zombie::zombieCounter;
Zombie::Zombie(GameObject& associated) : Component(associated) {
    hitpoints = 100;
    hit = false;
    deathSound = new Sound("Recursos/audio/Dead.wav");
    hitSound = new Sound("Recursos/audio/Hit0.wav");

    // SR
    SpriteRenderer* sprite = new SpriteRenderer(associated, "Recursos/img/Enemy.png", 3, 2);
    associated.AddComponent(sprite);

    // Animator
    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("walking", Animation(0, 3, 0.3));
    animator->AddAnimation("walkingLeft", Animation(0, 3, 0.3, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("hit", Animation(4, 4, 0));
    animator->AddAnimation("hitLeft", Animation(4, 4, 0, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(5, 5, 0));
    animator->AddAnimation("deadLeft", Animation(5, 5, 0, SDL_FLIP_HORIZONTAL));
    animator->SetAnimation("walking");

    zombieCounter++;
}

void Zombie::Start() {
    Collider* collider = new Collider(associated, Vec2(0.8, 1), Vec2(10, 0));
    associated.AddComponent(collider);
}

void Zombie::Damage(int damage) {
    // Already dead
    if (hitpoints <= 0) return;

    hitpoints -= damage;
    if (hitpoints <= 0) {
        Animator* animator = (Animator*) associated.GetComponent("Animator");
        animator->SetAnimation(walkingLeft ? "deadLeft" : "dead");
        associated.RemoveComponent(associated.GetComponent("Collider"));
        deathSound->Play(1);
        deathTimer.Restart();
    } else {
        Animator* animator = (Animator*) associated.GetComponent("Animator");
        hit = true;
        hitTimer.Restart();
        animator->SetAnimation(walkingLeft ? "hitLeft" : "hit");
        hitSound->Play(1);
    }
}

void Zombie::Update(float dt) {
    deathTimer.Update(dt);
    hitTimer.Update(dt);
    if (hitpoints > 0) {
        if (hit) {
            if (hitTimer.Get() >= 0.5) {
                hit = false;
            }
        } else if (Character::player != nullptr) {
            int speed = 75;
            Vec2 dir = Character::player->Pos().Sub(associated.box.Center()).Normalized();
            associated.box = associated.box.Add(dir.MulScalar(speed * dt));

            Animator* animator = (Animator*) associated.GetComponent("Animator");
            walkingLeft = dir.x < 0;
            animator->SetAnimation(walkingLeft ? "walkingLeft" : "walking");
        }
    } else if (deathTimer.Get() >= 5) {
        zombieCounter--;
        associated.RequestDelete();
    }
}

void Zombie::NotifyCollision(GameObject& other) {
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (bullet != nullptr) {
        Damage(bullet->GetDamage());
        hitSound->Play();
    }
}

void Zombie::Render() { }

bool Zombie::Is(std::string type) {
    return type == "Zombie";
}
