#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <string>
#include <memory>
#include <queue>
#include <vector>
#include <cmath>

#include "components/Animator.h"
#include "physics/Collider.h"
#include "physics/Collision.h"
#include "core/Component.h"
#include "physics/IsoCollider.h"
#include "components/SpriteRenderer.h"
#include "components/Sound.h"
#include "utils/Timer.h"
#include "math/Vec2.h"

#define RENDER_HITBOX 1  // Define para controlar a renderização da hitbox

class Character : public Component
{
public:
    Character(GameObject& associated, const char* sprite);
    ~Character();

    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject& other);

    class Command {
    public:
        enum CommandType { MOVE, ATTACK, SHOOT, TELEPORT };
        Command(CommandType type, float x, float y);
        CommandType type;
        Vec2 pos;
    };
    void Issue(Command task);

    void Hit(int damage);
    Vec2 Pos();

    int GetHP();

    SDL_Rect CalculateAttackBox(float x, float y, float angle);

    static Character* player;
private:
    int GetDirectionFromAngle(float angle);

    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    Vec2 speed;
    Vec2 lastMoveDirection;
    float linearSpeed;
    int hp;
    Timer deathTimer;
    Timer hitTimer;
    Timer attackTimer;
    Timer walkSoundTimer;
    //Timer recoveryTimer;
    bool flip;
    bool tookDamage;
    bool isAttacking;
    Sound* deathSound;
    Sound* hitSound;
    Sound* attackSound;
    Sound* walkSound;

    std::vector<std::string> idleSprites;
    std::vector<std::string> walkSprites;
    std::vector<std::string> attackSprites;
    std::vector<std::string> hitSprites;
    std::string currentSprite;
    int currentDirection;

    SDL_Rect attackBox;
};

#endif // CHARACTER_H
