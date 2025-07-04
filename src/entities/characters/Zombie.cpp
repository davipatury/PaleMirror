#include "entities/characters/Zombie.h"
#include "entities/projectiles/HitAttack.h"

int Zombie::zombieCounter;

Zombie::Zombie(GameObject& associated) : Component(associated) {
    hitpoints = 100;

    SpriteRenderer* sprite = new SpriteRenderer(associated,"Recursos/img/Monster/monster.png", 4, 1);
    associated.AddComponent(sprite);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("walking", Animation(0, 3, 0.3));
    animator->AddAnimation("walkingFlip", Animation(0, 3, 0.3, SDL_FLIP_HORIZONTAL));
    animator->SetAnimation("walking");
    
    zombieCounter++;
}

void Zombie::Start() {
    Collider* collider = new Collider(associated, Vec2(0.65, 1), Vec2(-5, 0));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-15, 15}, false);
    associated.AddComponent(isoCollider);
}

void Zombie::Damage(int damage) {
    if (hitpoints <= 0) return;
    hitpoints -= damage;
    if (hitpoints <= 0) {
        deathSound->Play(1);
        associated.RemoveComponent(associated.GetComponent("Collider"));
        deathTimer.Restart();
    } else {
        hitSound->Play(1);
    }
}

void Zombie::Update(float dt) {
    deathTimer.Update(dt);

    if (hitpoints > 0) {
        if (Character::player) {
            if (Character::player->Pos().Distance(associated.box.Center()) < 1000) {
                const int speed = 75;
                Vec2 dir = Character::player->Pos().Sub(associated.box.Center()).Normalized();
                associated.box = associated.box.Add(dir.MulScalar(speed * dt));

                Animator* animator = static_cast<Animator*>(associated.GetComponent("Animator"));
                bool goingLeft = dir.x < 0;
                animator->SetAnimation(goingLeft ? "walking" : "walkingFlip");
            }
        }
    } else if (deathTimer.Get() >= 5) {
        zombieCounter--;
        associated.RequestDelete();
    }

    // std::cout << "Zombie position: " << associated.box.x << ", " << associated.box.y << std::endl;
}

void Zombie::NotifyCollision(GameObject& other) {
    IsoCollider* colB = (IsoCollider*) other.GetComponent("IsoCollider");
    if (colB != nullptr && !colB->passable) {
        IsoCollider* colA = (IsoCollider*) associated.GetComponent("IsoCollider");
        Rect before = colA->box;
        Rect after = Collision::Solve(colA->box, colB->box, colA->prevBox);
        Vec2 diff = (after.TopLeft() - before.TopLeft()).ToCart();

        colA->box = after;
        associated.box = associated.box.Add(diff);
    }

    if (auto* bullet =
            static_cast<Bullet*>(other.GetComponent("Bullet"))) {
        Damage(bullet->GetDamage());
    }
    if (auto* hitAttack =
            static_cast<HitAttack*>(other.GetComponent("HitAttack"))) {
        Damage(hitAttack->GetDamage());
    }
}

void Zombie::Render() {}

bool Zombie::Is(std::string type) {
    return type == "Zombie";
}
