#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <string>
#include <memory>
#include <queue>

#include "Animator.h"
#include "Collider.h"
#include "Component.h"
#include "Gun.h"
#include "IsoCollider.h"
#include "SpriteRenderer.h"
#include "Sound.h"
#include "Timer.h"
#include "Vec2.h"

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
        enum CommandType { MOVE, SHOOT };
        Command(CommandType type, float x, float y);
        CommandType type;
        Vec2 pos;
    };
    void Issue(Command task);

    void Hit(int damage);
    Vec2 Pos();

    int GetHP();

    static Character* player;
private:
    std::weak_ptr<GameObject> gun;
    std::queue<Command> taskQueue;
    Vec2 speed;
    float linearSpeed;
    int hp;
    Timer deathTimer;
    bool flip;
    bool tookDamage;
    Timer damageTimer;
    Sound* deathSound;
    Sound* hitSound;
};

#endif // CHARACTER_H
