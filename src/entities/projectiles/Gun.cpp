#include "entities/projectiles/Gun.h"

#include "core/Game.h"

Gun::Gun(GameObject& associated, std::weak_ptr<GameObject> character) : Component(associated) {
    SpriteRenderer* spriteRdr = new SpriteRenderer(associated, "Recursos/img/Gun.png", 3, 2);
    associated.AddComponent(spriteRdr);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->AddAnimation("idleF", Animation(0, 0, 0, SDL_FLIP_VERTICAL));
    animator->AddAnimation("reloading", Animation(1, 5, 0.1));
    animator->AddAnimation("reloadingF", Animation(1, 5, 0.1, SDL_FLIP_VERTICAL));
    animator->SetAnimation("idle");

    this->character = character;
    angle = 0;
    cooldown = 0;

    shotSound = Sound("Recursos/audio/Range.wav");
    reloadSound = Sound("Recursos/audio/PumpAction.mp3");
}

void Gun::UpdateBox() {
    std::shared_ptr<GameObject> chr = character.lock();
    if (chr == nullptr) {
        associated.RequestDelete();
        return;
    }
    associated.angleDeg = angle * 180 / M_PI;
    associated.box = associated.box.Add(chr->box.Center().Sub(associated.box.Center()));
    associated.box = associated.box.Add(Vec2(50, 0).Rotate(angle));
}

void Gun::Update(float dt) {
    std::shared_ptr<GameObject> chr = character.lock();
    if (chr == nullptr) {
        associated.RequestDelete();
        return;
    }

    UpdateBox();
    cdTimer.Update(dt);

    Animator* animator = (Animator*) associated.GetComponent("Animator");
    switch (cooldown) {
        // Recarregando
        case 1: {
            if (cdTimer.Get() > 0.5) {
                cdTimer.Restart();
                animator->SetAnimation(associated.angleDeg > 90 || associated.angleDeg < -90 ? "reloadingF" : "reloading");
                reloadSound.Play();
                cooldown = 2;
            }
            break;
        }
        // Tempo parado
        case 2: {
            if (cdTimer.Get() > 0.5) {
                cdTimer.Restart();
                animator->SetAnimation(associated.angleDeg > 90 || associated.angleDeg < -90 ? "idleF" : "idle");
                cooldown = 0;
            }
            break;
        }
    }
}

void Gun::Render() { }

void Gun::Shoot(Vec2 target) {
    if (cooldown != 0) return;

    angle = target.Sub(associated.box.Center()).Angle();
    UpdateBox();
    Animator* animator = (Animator*) associated.GetComponent("Animator");
    animator->SetAnimation(associated.angleDeg > 90 || associated.angleDeg < -90 ? "idleF" : "idle");

    shotSound.Play();
    cooldown = 1;

    std::shared_ptr<GameObject> chr = character.lock();
    bool targetsPlayer = chr->GetComponent("Character") != Character::player;

    GameObject* bulletGO = new GameObject();
    Bullet* bullet = new Bullet((*bulletGO), angle, 300, 25, 1000, targetsPlayer);
    bulletGO->AddComponent(bullet);
    Vec2 offset = Vec2(30, 0).Rotate(angle);
    bulletGO->box.x = associated.box.Center().x + offset.x;
    bulletGO->box.y = associated.box.Center().y + offset.y;
    bulletGO->box.z = 2;
    bulletGO->angleDeg = angle * 180 / M_PI + 90;
    Game::GetInstance().GetCurrentState().AddObject(bulletGO);
}

bool Gun::Is(std::string type) {
    return type == "Gun";
}
