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
#include "hud/FlashlightHUD.h"
#include "states/StageState.h"

#define CHARACTER_SIZE 112

Character* Character::player = nullptr;

Character::Character(GameObject& associated, const char* sprite) : Component(associated) {
    linearSpeed = 160;
    hp = 1000;
    flip = false;
    lastMoveDirection = Vec2(1, 0);
    deathSound = new Sound("Recursos/audio/Dead.wav");
    hitSound = new Sound("Recursos/audio/sounds/Helena/hit1.wav");
    attackSound = new Sound("Recursos/audio/sounds/Helena/attack1.wav");
    walkSound = new Sound("Recursos/audio/sounds/Helena/passos.wav");

    int frameCountW = 6;
    int frameCountH = 15;
    SpriteRenderer* spriteRdr = new SpriteRenderer(associated, "Recursos/img/Helena/spritesheet.png", frameCountW, frameCountH);
    associated.AddComponent(spriteRdr);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);

    // Baixo-Esquerda (1)
    // Esquerda (2)
    // Cima-Esquerda (3)
    // Cima (4)
    // Cima-Direita (5)
    // Direita (6)
    // Baixo-Direita (7)
    // Baixo (8)
    for (int i = 0; i < 8; i++) {
        std::string n = std::to_string(i+1);
        // Walk
        SDL_RendererFlip walkFlip = (i+1 == 7 || i+1 == 6 || i+1 == 3) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        int walkFrameStart = i * frameCountW;
        int walkFrameEnd = walkFrameStart + 3;
        animator->AddAnimation("walking" + n, Animation(walkFrameStart, walkFrameEnd, 0.150, walkFlip));

        // Idle
        int idleOffset = 13 * frameCountW;
        SDL_RendererFlip idleFlip = (i+1 == 3 || i+1 == 4 || i+1 == 6 || i+1 == 7) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        int idleFrame = ((i+1 == 1 || i+1 == 2 || i+1 == 6 || i+1 == 7 || i+1 == 8) ? 0 : 1) + idleOffset;
        animator->AddAnimation("idle" + n, Animation(idleFrame, idleFrame, 0.250, idleFlip));

        // Hit
        int hitOffset = 11 * frameCountW;
        SDL_RendererFlip hitFlip = (i+1 == 5 || i+1 == 6 || i+1 == 7) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        int hitFrameStart = (i+1 == 3 || i+1 == 4 || i+1 == 5 ? 0 : frameCountW) + hitOffset;
        int hitFrameEnd = hitFrameStart + 2;
        animator->AddAnimation("hit" + n, Animation(hitFrameStart, hitFrameEnd, 0.5, hitFlip));

        // Attack
        int attackOffset = 8 * frameCountW;
        SDL_RendererFlip attackFlip = (i+1 == 7 || i+1 == 6 || i+1 == 3 || i+1 == 4) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        int attackFrameStart = ((i+1 == 2 || i+1 == 6) ? 0 : ((i+1 == 1 || i+1 == 7 || i+1 == 8) ? frameCountW : frameCountW*2)) + attackOffset;
        int attackFrameEnd = attackFrameStart + 5;
        animator->AddAnimation("attack" + n, Animation(attackFrameStart, attackFrameEnd, 0.120, attackFlip));
    }

    int deadFrameStart = 14 * frameCountW;
    int deadFrameEnd = deadFrameStart + 5;
    animator->AddAnimation("dead", Animation(deadFrameStart, deadFrameEnd, 0.5));

    currentDirection = 1;
    animator->SetAnimation("idle1");

    isAttacking = false;
    deathTimer = Timer();
    hitTimer = Timer();
    attackTimer = Timer();
    walkSoundTimer = Timer();
    //recoveryTimer = Timer();
}

Character::~Character() {
    if (this == player) {
        Camera::Unfollow();
        player = nullptr;
    }
    delete deathSound;
    delete hitSound;
    delete attackSound;
    delete walkSound;
}

void Character::Start() {
    Collider* collider = new Collider(associated, Vec2(0.5, 1));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-5, 0}, false);
    associated.AddComponent(isoCollider);
}

void Character::Update(float dt) {
    Animator* animator = (Animator*) associated.GetComponent("Animator");
    SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    if (spriteRdr == nullptr || animator == nullptr) return;

    bool moving = false;
    while (hp > 0 && !taskQueue.empty()) {
        Command task = taskQueue.front();
        switch (task.type) {
        case Command::MOVE: {
            if (!isAttacking and !tookDamage) {
                if(walkSoundTimer.Get() > 0.5f) {
                    walkSoundTimer.Restart();
                    walkSound->Play(0);
                }
                Vec2 moveSpeed = task.pos.Normalized().MulScalar(linearSpeed * dt);
                if (moveSpeed.x != 0 && moveSpeed.y != 0) {
                    // Straighten diagonal movement (xSpeed = 2 * ySpeed)
                    moveSpeed.x *= 1.5;
                    moveSpeed.y *= 0.8671875;
                }
                if (moveSpeed.x != 0 && moveSpeed.y == 0) {
                    moveSpeed.x *= 1.1;
                }

                // Room limits
                StageState* state = (StageState*) &CURRENT_STATE;
                Rect limits = state->GetCurrentRoom()->cameraLimits;
                Vec2 targetPos = associated.box.TopLeft().Add(moveSpeed);

                if (limits.w != 0) {
                    float minX = limits.x;
                    float maxX = limits.BottomRight().x - associated.box.w;
                    float minY = limits.y;
                    float maxY = limits.BottomRight().y - associated.box.h;
                    if (targetPos.x < minX) targetPos.x = minX;
                    if (targetPos.y < minY) targetPos.y = minY;
                    if (targetPos.x > maxX) targetPos.x = maxX;
                    if (targetPos.y > maxY) targetPos.y = maxY;
                }

                associated.box.x = targetPos.x;
                associated.box.y = targetPos.y;
                moving = true;
                if (moveSpeed.x != 0 || moveSpeed.y != 0) {
                    lastMoveDirection = task.pos.Normalized();
                }
            }
            break;
        }
        case Command::ATTACK: {
            if (!isAttacking and !tookDamage) {
                isAttacking = true;
                attackTimer.Restart();
                attackSound->Play(0);
                float angle = atan2(lastMoveDirection.y, lastMoveDirection.x);
                IsoCollider* Col = (IsoCollider*) associated.GetComponent("IsoCollider");
                Vec2 coordsCenter = Col->box.Center().ToCart();
                int centerX = coordsCenter.x;
                int centerY = coordsCenter.y;
                attackBox = CalculateAttackBox(centerX, centerY, angle);
                
                GameObject* attackGO = new GameObject();
                attackGO->box.x = attackBox.x;
                attackGO->box.y = attackBox.y;
                attackGO->box.w = attackBox.w;
                attackGO->box.h = attackBox.h;
                
                HitAttack* hitAttack = new HitAttack(*attackGO, 35, 0.8f);
                attackGO->AddComponent(hitAttack);
                
                CURRENT_STATE.AddObject(attackGO);

                animator->SetAnimation("attack" + std::to_string(currentDirection + 1));
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
    hitTimer.Update(dt);
    attackTimer.Update(dt);
    walkSoundTimer.Update(dt);
    //recoveryTimer.Update(dt);

    if (tookDamage) {
        if (hitTimer.Get() > 0.2f) tookDamage = false;
    } else {
        if (hp > 0) {
            //std::string animName;
            float angle = atan2(lastMoveDirection.y, lastMoveDirection.x) * 180 / M_PI;
            currentDirection = GetDirectionFromAngle(angle);
            std::string n = std::to_string(currentDirection + 1);

            if (isAttacking) {
                if (attackTimer.Get() > 0.8) {
                    isAttacking = false;
                    if (moving) animator->SetAnimation("walk" + n);
                    else        animator->SetAnimation("idle" + n);
                }
            } else if (moving) {
                animator->SetAnimation("walking" + n);
            } else {
                animator->SetAnimation("idle" + n);
            }
        } else {
            if (deathTimer.Get() > 3) {
                associated.RequestDelete();
            }
        }
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
            Hit(20);
            return;
        }
        if (other.GetComponent("ZombieFast") != nullptr && this == player) {
            Hit(10);
            return;
        }
        if (other.GetComponent("HitAttack") != nullptr && this == player) {
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
    
}

void Character::Hit(int damage) {
    if (hp <= 0) return;
    if(hitTimer.Get() < 1.2f) return;
    hp -= damage;
    
    Animator* animator = (Animator*) associated.GetComponent("Animator");
    if (hp <= 0) {
        dying = true;
        if (animator) animator->SetAnimation("dead");
        deathTimer.Restart();
        deathSound->Play();
    } else {
        hitTimer.Restart();
        tookDamage = true;
        hitSound->Play(0);
        Animator* animator = (Animator*) associated.GetComponent("Animator");
        if (animator) animator->SetAnimation("hit" + std::to_string(currentDirection + 1));
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
    float distance = 55;
    int size = 80;
    
    float centerX = x + distance * cos(angle);
    float centerY = y + distance * sin(angle);
    
    box.x = centerX - size/2;
    box.y = centerY - size/2;
    box.w = size;
    box.h = size;
    
    return box;
}

void Character::Render() {}

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

bool Character::IsDying() {
    return dying;
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
