#include "entities/characters/Boss.h"
#include "core/Game.h"
#include "core/GameData.h"
#include "entities/characters/Zombie.h"
#include "entities/characters/ZombieFast.h"
#include "hud/DialogueHUD.h"

bool Boss::startBoss = false;
int Boss::currentzombies = 0;

Boss::Boss(GameObject& associated) : Component(associated){
    SpriteRenderer* sprite = new SpriteRenderer(associated,"Recursos/img/Monster/aneleh.png", 8, 1);
    associated.AddComponent(sprite);
    summonSound = new Sound("Recursos/audio/sounds/monster/summon.wav");

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("summon", Animation(0, 7, 0.400));
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->SetAnimation("idle");
    
    summonTimer.Restart();
    summoningTimer.Restart();
    summoning = false;
    firstsummon = true;
}

void Boss::Start() {
    Collider* collider = new Collider(associated, Vec2(0.65, 1), Vec2(-5, 0));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-15, 15}, false);
    associated.AddComponent(isoCollider);
}

void Boss::Update(float dt) {
    if(!DialogueHUD::isEmpty()) return;
    summonTimer.Update(dt);
    summoningTimer.Update(dt);

    if((summonTimer.Get() >= 5 or firstsummon) and currentzombies<maxzombies){
        summonTimer.Restart();
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        animator->SetAnimation("summon");
        summoning = true;
        firstsummon = false;
        summoningTimer.Restart();

        summonSound->Play();
    }

    if(summoning and summoningTimer.Get() >= 3 ){
        summoning = false;
        summoningTimer.Restart();
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        animator->SetAnimation("idle");
        
        // Summon zombies

        int numZombies = 4;
        float radius = 100.0f;
        float angleStep = 360.0f / numZombies;

        for (int i = 0; i < numZombies && currentzombies<maxzombies; i++) {
            float angle = angleStep * i;
            float rad = angle * M_PI / 180.0f;

            auto col = dynamic_cast<IsoCollider*>(associated.GetComponent("IsoCollider"));

            float zombieX = col->box.Center().ToCart().x + radius * cos(rad);
            float zombieY = col->box.Center().ToCart().y + radius * sin(rad);

            GameObject* zombieGO = new GameObject();
            if(i < numZombies-1){
                Zombie* zombie = new Zombie(*zombieGO);
                zombieGO->AddComponent(zombie);
                CURRENT_STATE.AddObject(zombieGO);
            }else{
                ZombieFast* zombie = new ZombieFast(*zombieGO);
                zombieGO->AddComponent(zombie);
                CURRENT_STATE.AddObject(zombieGO);
            }
            auto spt = dynamic_cast<SpriteRenderer*>(associated.GetComponent("SpriteRenderer"));
            zombieGO->box.x = zombieX - spt->sprite.GetWidth()/2;
            zombieGO->box.y = zombieY - spt->sprite.GetHeight();
            
            currentzombies++;
        }

        CURRENT_STATE.Pause();
    }
}

void Boss::NotifyCollision(GameObject& other) {
    IsoCollider* colB = (IsoCollider*) other.GetComponent("IsoCollider");
    if (colB != nullptr && !colB->passable) {
        IsoCollider* colA = (IsoCollider*) associated.GetComponent("IsoCollider");
        Rect before = colA->box;
        Rect after = Collision::Solve(colA->box, colB->box, colA->prevBox);
        Vec2 diff = (after.TopLeft() - before.TopLeft()).ToCart();

        colA->box = after;
        associated.box = associated.box.Add(diff);
    }
}

void Boss::Render() {
    
}

bool Boss::Is(std::string type) {
    return type == "Boss";
}
