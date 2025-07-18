#ifndef BOSS_H
#define BOSS_H

#include <iostream>
#include <string>

#include "utils/Animation.h"
#include "components/Animator.h"
#include "components/Camera.h"
#include "physics/Collider.h"
#include "physics/Collision.h"
#include "core/Component.h"
#include "core/GameObject.h"
#include "utils/InputManager.h"
#include "components/SpriteRenderer.h"
#include "components/Sound.h"
#include "utils/Timer.h"



class Boss : public Component
{
public:
    Boss(GameObject& associated);

    void Start();
    void Update(float dt);
    void Render();

    void NotifyCollision(GameObject& other);

    bool Is(std::string type);

    static bool startBoss;
    static int currentzombies;

    Sound* summonSound;
    
private:
    int maxzombies = 10;

    bool summoning;
    bool firstsummon;
    Timer summonTimer;
    Timer summoningTimer;
};

#endif // BOSS_H
