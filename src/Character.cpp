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
    std::cout << "COLL " << associated.box.x << std::endl;

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
    /*
    Vec2 pos = Vec2(associated.box.x, associated.box.y) - Camera::pos;
    std::cout << "Pos " << pos.x << ", " << pos.y << std::endl;
    auto iso_pos = [](float x, float y, float w, float h) {
        return Vec2((x * 0.5 * w) - (y * 0.5 * w), (x * 0.25 * h) + (y * 0.25 * h));
    };
    Vec2 pos1 = iso_pos(pos.x, pos.y, associated.box.w, associated.box.h);
    std::cout << "Pos1 " << pos1.x << ", " << pos1.y << std::endl;
    Vec2 pos2 = iso_pos(pos.x + associated.box.w, pos.y, associated.box.w, associated.box.h);
    SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(GAME_RENDERER, pos1.x, pos1.y, pos2.x, pos2.y);
*/
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
