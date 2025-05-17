#include "Character.h"

#include "Game.h"

Character* Character::player;
Character::Character(GameObject& associated, const char* sprite) : Component(associated) {
    linearSpeed = 100;
    hp = 100;
    flip = false;
    deathSound = new Sound("Recursos/audio/Dead.wav");
    hitSound = new Sound("Recursos/audio/Hit1.wav");

    SpriteRenderer* spriteRdr = new SpriteRenderer(associated, sprite, 3, 4);
    associated.AddComponent(spriteRdr);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("idle", Animation(6, 9, 0.5));
    animator->AddAnimation("idleF", Animation(6, 9, 0.5, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("walking", Animation(0, 5, 0.2));
    animator->AddAnimation("walkingF", Animation(0, 5, 0.2, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("dead", Animation(10, 11, 0.5));
    animator->SetAnimation("idle");
}

Character::~Character() {
    if (this == player) {
        Camera::Unfollow();
        player = nullptr;
    }
}

void Character::Start() {
    GameObject* gunGO = new GameObject();
    Gun* gunComp = new Gun((*gunGO), Game::GetInstance().GetCurrentState().GetObjectPtr(&associated));
    gunGO->AddComponent(gunComp);
    gun = Game::GetInstance().GetCurrentState().AddObject(gunGO);

    Collider* collider = new Collider(associated);
    associated.AddComponent(collider);

    IsoCollider* isoCollider = new IsoCollider(associated, {1, 1}, {0, 0}, false);
    associated.AddComponent(isoCollider);
}

void Character::Update(float dt) {
    bool moving = false;
    while (hp > 0 && !taskQueue.empty()) {
        Command task = taskQueue.front();
        switch (task.type) {
        case Command::MOVE: {
            Vec2 moveSpeed = task.pos.Normalized().MulScalar(linearSpeed * dt);
            associated.box = associated.box.Add(moveSpeed);
            moving = true;
            flip = moveSpeed.x < 0;

            if (this == player) {
                if (associated.box.x < 640) associated.box.x = 640;
                if (associated.box.y < 512) associated.box.y = 512;
                float maxX = 1920 - associated.box.w;
                float maxY = 2048 - associated.box.h;
                if (associated.box.x > maxX) associated.box.x = maxX;
                if (associated.box.y > maxY) associated.box.y = maxY;
            }
            break;
        }
        case Command::SHOOT: {
            Gun* gunComp = (Gun*) gun.lock().get()->GetComponent("Gun");
            gunComp->Shoot(task.pos);
            break;
        }
        }
        taskQueue.pop();
    }

    deathTimer.Update(dt);
    damageTimer.Update(dt);

    Animator* animator = (Animator*) associated.GetComponent("Animator");
    if (hp > 0) {
        animator->SetAnimation(moving ? (flip ? "walkingF" : "walking") : (flip ? "idleF" : "idle"));
    } else {
        if (deathTimer.Get() > 3) {
            associated.RequestDelete();
        } else {
            animator->SetAnimation("dead");
        }
    }

    if (tookDamage && damageTimer.Get() > 1) {
        tookDamage = false;
    }
}

void Character::NotifyCollision(GameObject& other) {
    IsoCollider* colB = (IsoCollider*) other.GetComponent("IsoCollider");
    if (colB != nullptr && !colB->passable) {
        IsoCollider* colA = (IsoCollider*) associated.GetComponent("IsoCollider");
        Rect before = colA->box;
        Rect after = Collision::Solve(colA->box, colB->box, colA->prevBox);
        Vec2 diff = (after.TopLeft() - before.TopLeft()).ToCart();

        colA->box = after;
        associated.box = associated.box.Add(diff);
    }

    if (tookDamage || hp <= 0) return;
    if (other.GetComponent("Zombie") != nullptr && this == player) {
        Hit(20);
    }

    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (bullet != nullptr) {
        if ((this == player && bullet->targetsPlayer) || (this != player && !bullet->targetsPlayer)) {
            Hit(bullet->GetDamage() * (bullet->targetsPlayer ? 0.7 : 1));
        }
    }
}

void Character::Hit(int damage) {
    if (hp <= 0) return;

    hp -= damage;
    if (hp <= 0) {
        deathTimer.Restart();
        deathSound->Play();
    } else {
        damageTimer.Restart();
        tookDamage = true;
        hitSound->Play();
    }
}

Vec2 Character::Pos() {
    return associated.box.Center();
}

int Character::GetHP() {
    return hp;
}

void Character::Render() {
}

void Character::Issue(Command task) {
    taskQueue.push(task);
}

bool Character::Is(std::string type) {
    return type == "Character";
}

Character::Command::Command(CommandType type, float x, float y) {
    this->type = type;
    pos.x = x;
    pos.y = y;
}
