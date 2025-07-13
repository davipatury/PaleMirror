#include "entities/characters/Character.h"
#include "core/Game.h"
#include "entities/projectiles/HitAttack.h"
#include "components/SpriteRenderer.h"
#include "components/Animator.h"
#include "components/Sound.h"
#include "components/Camera.h"
#include "physics/Collider.h"
#include "physics/IsoCollider.h"
#include "physics/Collision.h"
#include "math/Vec2.h"
#include "math/Rect.h"
#include "utils/Timer.h"
#include "utils/Animation.h"

Character* Character::player = nullptr;

Character::Character(GameObject& associated, const char* sprite) : Component(associated) {
    linearSpeed = 150;
    hp = 100;
    flip = false;
    lastMoveDirection = Vec2(1, 0);
    deathSound = new Sound("Recursos/audio/Dead.wav");
    hitSound = new Sound("Recursos/audio/Hit1.wav");

    const std::string basePath = "Recursos/img/Helena/";
    const std::string types[] = {"Idle", "Walk", "Attack"};

    for (int i = 0; i < 8; i++) {
        // 1, 2, 6, 7, 8 -> 1
        // 3, 4, 5 -> 2 -> 2

        if(i == 1 || i == 5){
            attackSprites.push_back(basePath + types[2] + "/" + types[2] + "26" + ".png");
        }else if(i == 0 || i == 6 || i == 7){
            attackSprites.push_back(basePath + types[2] + "/" + types[2] + "178" + ".png");
        }else{
            attackSprites.push_back(basePath + types[2] + "/" + types[2] + "345" + ".png");
        }
        if(i == 0 || i == 1 || i == 5 || i == 6 || i == 7) {
            idleSprites.push_back(basePath + types[0] + "/" + types[0] + "1" + ".png");
        } else {
            idleSprites.push_back(basePath + types[0] + "/" + types[0] + "2" + ".png");
        }
        //idleSprites.push_back(basePath + types[0] + "/Knight_" + types[0] + "_dir" + std::to_string(i+1) + ".png");
        walkSprites.push_back(basePath + types[1] + "/" + types[1] + std::to_string(i+1) + ".png");
        //attackSprites.push_back(basePath + types[2] + "/Knight_" + types[2] + "_dir" + std::to_string(i+1) + ".png");
    }

    SpriteRenderer* spriteRdr = new SpriteRenderer(associated, idleSprites[7].c_str(), 1, 1);
    spriteRdr->SetScale(1, 1);
    spriteRdr->SetScale(0.5, 0.5);
    associated.AddComponent(spriteRdr);
    currentSprite = idleSprites[7];
    currentDirection = 7;

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    
    for (int i = 0; i < 8; i++) {
        if(i+1 == 7 or i+1 == 6 or i+1 == 3){
            animator->AddAnimation("walking" + std::to_string(i+1), Animation(0, 3, 0.250, SDL_FLIP_HORIZONTAL));
        }else{
            animator->AddAnimation("walking" + std::to_string(i+1), Animation(0, 3, 0.250));
        }

        // 1, 2, 8 = Parado
        // 3 e 4 = ParadoCostaLeft
        // 5 = ParadoCostaRight
        // 6 e 7 = ParadoRight
        if( i+1 == 1 or i+1 == 2 or i+1 == 8){
            animator->AddAnimation("idle" + std::to_string(i+1), Animation(0, 0, 0.250));
        }else if (i+1 == 3 or i+1 == 4){
            animator->AddAnimation("idle" + std::to_string(i+1), Animation(0, 0, 0.100, SDL_FLIP_HORIZONTAL));
        }else if(i+1 == 5){
            animator->AddAnimation("idle" + std::to_string(i+1), Animation(0, 0, 0.250));
        }else{
            animator->AddAnimation("idle" + std::to_string(i+1), Animation(0, 0, 0.250, SDL_FLIP_HORIZONTAL));
        }

        if(i+1 == 7 || i+1 == 6 || i+1 == 3 || i+1 == 4){
            animator->AddAnimation("attack" + std::to_string(i+1), Animation(0, 5, 0.150, SDL_FLIP_HORIZONTAL));
        }else{
            animator->AddAnimation("attack" + std::to_string(i+1), Animation(0, 5, 0.150));
        }

        //animator->AddAnimation("idle" + std::to_string(i+1), Animation(0, 3, 0.250));
        //animator->AddAnimation("attack" + std::to_string(i+1), Animation(0, 14, 0.1));
    }

    animator->AddAnimation("dead", Animation(0, 0, 0.5));
    animator->SetAnimation("idle1");

    isAttacking = false;
    deathTimer = Timer();
    damageTimer = Timer();
    attackTimer = Timer();
    //recoveryTimer = Timer();
}

Character::~Character() {
    if (this == player) {
        Camera::Unfollow();
        player = nullptr;
    }
    delete deathSound;
    delete hitSound;
}

void Character::Start() {
    Collider* collider = new Collider(associated, Vec2(0.5, 1));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-5, 0}, false);
    associated.AddComponent(isoCollider);
}

void Character::Update(float dt) {
    bool moving = false;
    while (hp > 0 && !taskQueue.empty()) {
        Command task = taskQueue.front();
        switch (task.type) {
        case Command::MOVE: {
            if (!isAttacking) {
                Vec2 moveSpeed = task.pos.Normalized().MulScalar(linearSpeed * dt);
                if (moveSpeed.x != 0 && moveSpeed.y != 0) {
                    // Straighten diagonal movement (xSpeed = 2 * ySpeed)
                    // TODO: use 0.578125 instead of dividing/multiplying by 2
                    moveSpeed.x *= 1.5;
                    moveSpeed.y *= 0.8671875;
                }
                if (moveSpeed.x != 0 && moveSpeed.y == 0) {
                    moveSpeed.x *= 1.1;
                }
                associated.box = associated.box.Add(moveSpeed);
                moving = true;
                if (moveSpeed.x != 0 || moveSpeed.y != 0) {
                    lastMoveDirection = task.pos.Normalized();
                }
            }
            break;
        }
        case Command::ATTACK: {
            if (!isAttacking) {
                isAttacking = true;
                attackTimer.Restart();
                float angle = atan2(lastMoveDirection.y, lastMoveDirection.x);
                int centerX = (int)(associated.box.x + associated.box.w/2);
                int centerY = (int)(associated.box.y + associated.box.h/2);
                attackBox = CalculateAttackBox(centerX, centerY, angle);
                
                GameObject* attackGO = new GameObject();
                attackGO->box.x = attackBox.x;
                attackGO->box.y = attackBox.y;
                attackGO->box.w = attackBox.w;
                attackGO->box.h = attackBox.h;
                
                HitAttack* hitAttack = new HitAttack(*attackGO, 35, 1);
                attackGO->AddComponent(hitAttack);
                
                CURRENT_STATE.AddObject(attackGO);
            }
            break;
        }
        case Command::TELEPORT: {
            associated.box.x = task.pos.x;
            associated.box.y = task.pos.y;
            break;
        }
        }
        taskQueue.pop();
    }

    deathTimer.Update(dt);
    damageTimer.Update(dt);
    attackTimer.Update(dt);

    //recoveryTimer.Update(dt);

    Animator* animator = (Animator*) associated.GetComponent("Animator");
    SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    spriteRdr->SetScale(0.085, 0.085);
    
    if (hp > 0) {
        std::string animName;
        float angle = atan2(lastMoveDirection.y, lastMoveDirection.x) * 180 / M_PI;
        currentDirection = GetDirectionFromAngle(angle);
        
        if (isAttacking) {
            animName = "attack" + std::to_string(currentDirection + 1);

            if (attackTimer.Get() > 1.0) {
                isAttacking = false;
                if (spriteRdr) {
                    if (moving) {
                        spriteRdr->Open(walkSprites[currentDirection].c_str());
                        spriteRdr->SetFrameCount(4, 1);
                        currentSprite = walkSprites[currentDirection];
                    } else {
                        spriteRdr->Open(idleSprites[currentDirection].c_str());
                        spriteRdr->SetFrameCount(1, 1);
                        currentSprite = idleSprites[currentDirection];
                    }
                }
            }else{
                if (spriteRdr && currentSprite != attackSprites[currentDirection]) {
                    spriteRdr->Open(attackSprites[currentDirection].c_str());
                    spriteRdr->SetFrameCount(6, 1);
                    currentSprite = attackSprites[currentDirection];
                }
            }
        } else if (moving) {
            animName = "walking" + std::to_string(currentDirection + 1);

            if (spriteRdr && currentSprite != walkSprites[currentDirection]) {
                    spriteRdr->Open(walkSprites[currentDirection].c_str());
                    spriteRdr->SetFrameCount(4, 1);
                    currentSprite = walkSprites[currentDirection];
            }
        } else {
            animName = "idle" + std::to_string(currentDirection + 1);

            if (spriteRdr && currentSprite != idleSprites[currentDirection]) {
                spriteRdr->Open(idleSprites[currentDirection].c_str());
                spriteRdr->SetFrameCount(1, 1);
                currentSprite = idleSprites[currentDirection];
            }
        }
        animator->SetAnimation(animName);
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

    /*
    if(recoveryTimer.Get() > 20){
        hp += 25;
        hp = std::min(hp, 100);
        recoveryTimer.Restart();
    }
    */

    //std::cout << "Player position: " << associated.box.x << ", " << associated.box.y << std::endl;
}

void Character::NotifyCollision(GameObject& other) {
    IsoCollider* colB = (IsoCollider*) other.GetComponent("IsoCollider");
    if (colB != nullptr && !colB->passable) {
        if (other.GetComponent("Zombie") != nullptr && this == player) {
            Hit(10);
            return;
        }
        IsoCollider* colA = (IsoCollider*) associated.GetComponent("IsoCollider");
        Rect before = colA->box;
        Rect after = Collision::Solve(colA->box, colB->box, colA->prevBox);
        Vec2 diff = (after.TopLeft() - before.TopLeft()).ToCart();

        colA->box = after;
        associated.box = associated.box.Add(diff);
        

    }
    if (tookDamage || hp <= 0) return;
    
    Bullet* bullet = (Bullet*) other.GetComponent("Bullet");
    if (bullet != nullptr) {
        if ((this == player && bullet->targetsPlayer) || (this != player && !bullet->targetsPlayer)) {
            Hit(bullet->GetDamage() * (bullet->targetsPlayer ? 0.7 : 1));
        }
    }
}

void Character::Hit(int damage) {
    if (hp <= 0) return;

    if(damageTimer.Get() < 1) return;
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

SDL_Rect Character::CalculateAttackBox(float x, float y, float angle) {
    SDL_Rect box;
    float distance = 50;
    int size = 70;
    
    float centerX = x + distance * cos(angle);
    float centerY = y + distance * sin(angle);
    
    float perspectiveOffset = 0;
    float normalizedAngle = angle * 180.0f / M_PI;
    if (normalizedAngle < 0) normalizedAngle += 360;
    
    if (normalizedAngle >= 0 && normalizedAngle <= 180) {
        perspectiveOffset = -50;
    }
    
    box.x = centerX - size/2;
    box.y = centerY - size/2 + perspectiveOffset;
    box.w = size;
    box.h = size;
    
    return box;
}

void Character::Render() {
    if (isAttacking && attackBox.w > 0 && attackBox.h > 0) {
        SDL_Rect destRect = {
            static_cast<int>(attackBox.x - Camera::pos.x),
            static_cast<int>(attackBox.y - Camera::pos.y),
            attackBox.w,
            attackBox.h
        };
        
        SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(GAME_RENDERER, &destRect);
    }
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

int Character::GetDirectionFromAngle(float angle) {
    if (angle < 0) angle += 360;
    
    if (angle >= 337.5 || angle < 22.5) return 5;      // Direita (6)
    if (angle >= 22.5 && angle < 67.5) return 6;       // Baixo-Direita (7)
    if (angle >= 67.5 && angle < 112.5) return 7;      // Baixo (8)
    if (angle >= 112.5 && angle < 157.5) return 0;     // Baixo-Esquerda (1)
    if (angle >= 157.5 && angle < 202.5) return 1;     // Esquerda (2)
    if (angle >= 202.5 && angle < 247.5) return 2;     // Cima-Esquerda (3)
    if (angle >= 247.5 && angle < 292.5) return 3;     // Cima (4)
    return 4;                                          // Cima-Direita (5)
}
