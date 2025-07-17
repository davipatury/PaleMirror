#include "entities/characters/Boss.h"
#include "core/Game.h"
#include "core/GameData.h"

Boss::Boss(GameObject& associated) : Component(associated){
    SpriteRenderer* sprite = new SpriteRenderer(associated,"Recursos/img/Monster/aneleh.png", 8, 1);
    associated.AddComponent(sprite);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("summon", Animation(0, 7, 0.5));
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->SetAnimation("idle");
    
    summonTimer.Restart();
    summoning = false;
    summoningTimer.Restart();
}

void Boss::Start() {
    Collider* collider = new Collider(associated, Vec2(0.65, 1), Vec2(-5, 0));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-15, 15}, false);
    associated.AddComponent(isoCollider);
}

void Boss::Update(float dt) {
    summonTimer.Update(dt);
    summoningTimer.Update(dt);

    if(summonTimer.Get() >= 2){
        summonTimer.Restart();
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        animator->SetAnimation("summon");
        summoning = true;
        summoningTimer.Restart();

        // summonSound.Play(0);
    }

    if(summoning and summoningTimer.Get() >= 3){
        summoning = false;
        summoningTimer.Restart();
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        animator->SetAnimation("idle");
        
        // Summon zombies

        /*
        for(int i = 0; i < 5; i++){
            GameObject* zombieGO = new GameObject();
            zombieGO->box.x = associated.box.x + (i * 50);
            zombieGO->box.y = associated.box.y + (i * 50);
            Zombie* zombie = new Zombie(*zombieGO);
            zombieGO->AddComponent(zombie);
            CURRENT_STATE.AddObject(zombieGO);
        }
        */
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
