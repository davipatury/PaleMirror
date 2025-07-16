#include "entities/characters/ZombieFast.h"
#include "entities/projectiles/HitAttack.h"
#include "core/Game.h"
#include "core/GameData.h"

int ZombieFast::zombieCounter;

ZombieFast::ZombieFast(GameObject& associated) : Component(associated), deathSound("Recursos/audio/Dead.wav"), hitSound("Recursos/audio/sounds/monster/hit1.wav"), monsterSound("Recursos/audio/sounds/monster/monstro500-2.wav") {
    hitpoints = 100;
    SpriteRenderer* sprite = new SpriteRenderer(associated,"Recursos/img/Monster/zombiefastidle", 1, 1);
    associated.AddComponent(sprite);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("idle", Animation(0, 0, 0));
    animator->AddAnimation("walkingFlip", Animation(0, 3, 0.1));
    animator->AddAnimation("walking", Animation(0, 3, 0.1, SDL_FLIP_HORIZONTAL));
    animator->AddAnimation("hit", Animation(0, 1, 0.250));
    animator->SetAnimation("idle");
    currentSprite = "idle";
    
    zombieCounter++;
    hit = false;
    hitTimer.Restart();
    deathTimer.Restart();
    monsterSoundTimer.Restart();
}

void ZombieFast::Start() {
    Collider* collider = new Collider(associated, Vec2(0.65, 1), Vec2(-5, 0));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-15, 15}, false);
    associated.AddComponent(isoCollider);
}

void ZombieFast::Damage(int damage) {
    if (hitpoints <= 0) return;
    hitpoints -= damage;
    if (hitpoints <= 0) {
        deathSound.Play(1);
        associated.RemoveComponent(associated.GetComponent("IsoCollider"));
        deathTimer.Restart();
    } else {
        hitSound.Play(0);
        hit = true;
        hitTimer.Restart();

        /*
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
        spriteRdr->Open("Recursos/img/Monster/monsterHit.png");
        spriteRdr->SetFrameCount(2, 1);
        animator->SetAnimation("hit");
        */
    }
}

void ZombieFast::Update(float dt) {
    deathTimer.Update(dt);
    hitTimer.Update(dt);
    monsterSoundTimer.Update(dt);

    if(monsterSoundTimer.Get() > ((rand()%6)+4) && hitpoints > 0) {
        if (Character::player and Character::player->Pos().Distance(associated.box.Center()) <= chaseRadius) {
            monsterSound.Play(0);
            monsterSoundTimer.Restart();
        }
    }

    if(Character::player){
        float dist = Character::player->Pos().Distance(associated.box.Center());
        if(dist >= chaseRadius and dist <= 3*chaseRadius) {
            GameData::zombieFarAway = true;
        }
    }

    if(hit and hitpoints and hitTimer.Get() > 1.5f){
        auto animator = dynamic_cast<Animator*>(associated.GetComponent("Animator"));
        SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
        hit = false;
    }

    if (hitpoints > 0) {
        if (Character::player) {
            Vec2 playerPos = Character::player->Pos();
            Vec2 selfPos = associated.box.Center();
            float dist = playerPos.Distance(selfPos);

            if (dist > chaseRadius){
                Animator* anim = static_cast<Animator*>(associated.GetComponent("Animator"));
                SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
                spriteRdr->Open("Recursos/img/Monster/zombiefastidle.png");
                spriteRdr->SetFrameCount(1, 1);
                anim->SetAnimation("idle");
                currentSprite = "idle";
                return;
            }

            // calcula ou atualiza caminho
            std::vector<Vec2> maybePath = computePath(playerPos);

            if (!maybePath.size()) return;
            if (maybePath.size() != path.size() || (path.size() > 0 && maybePath[0] != path[0])) {
                swap(path, maybePath);
                pathIndex = 0;
            }
            if(hit) return;
            // segue próximo ponto do caminho
            if (pathIndex < path.size()) {
                Vec2 target = path[pathIndex];
                Vec2 dir = target.Sub(selfPos).Normalized();
                associated.box = associated.box.Add(dir.MulScalar(speed * dt));
                if (associated.box.Center().Distance(target) < speed * dt * 0.5f) pathIndex++;

                Animator* anim = static_cast<Animator*>(associated.GetComponent("Animator"));
                bool left = dir.x < 0;
                SpriteRenderer* spriteRdr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");

                if(left){
                    if (spriteRdr && currentSprite != "walkingFlip") {
                        spriteRdr->Open("Recursos/img/Monster/zombiefastwalk.png");
                        spriteRdr->SetFrameCount(4, 1);
                        anim->SetAnimation("walkingFlip");
                        currentSprite = "walkingFlip";
                    }
                }else{
                    if (spriteRdr && currentSprite != "walking") {
                        spriteRdr->Open("Recursos/img/Monster/zombiefastwalk.png");
                        spriteRdr->SetFrameCount(4, 1);
                        anim->SetAnimation("walking");
                        currentSprite = "walking";
                    }
                }
            }
        }
    } else if (deathTimer.Get() >= 2) {
        zombieCounter--;
        associated.RequestDelete();
    }

    

    // std::cout << "ZombieFast position: " << associated.box.x << ", " << associated.box.y << std::endl;
}

bool ZombieFast::checkCollision(float x, float y) {
    auto oA = CURRENT_STATE.objectArray;
    IsoCollider* colliderB = (IsoCollider*) associated.GetComponent("IsoCollider");
    // x e y é o centro
    auto isoBox = colliderB->MakeIsoBoxForPoint(x - associated.box.w/2, y - associated.box.h/2);
    isoB = isoBox;
    
    for (int i = 0; i < oA.size(); i++) {
        GameObject* go = oA[i].get();
        if (go == &associated) continue;
        IsoCollider* colliderA = (IsoCollider*) go->GetComponent("IsoCollider");
        if(colliderA != nullptr and !colliderA->passable and colliderA->box.Collides(isoBox) and (go->GetComponent("Character") == nullptr)) {
            return true;
        }
    }
    return false;
}

static inline int64_t pointKey(const Vec2& v, float step) {
    int ix = int(std::floor(v.x / step));
    int iy = int(std::floor(v.y / step));
    return (int64_t(ix) << 32) | uint32_t(iy);
}

std::vector<Vec2> ZombieFast::computePath(const Vec2& target) {
    Vec2 start = associated.box.Center();

    auto cmp = [&target](const Vec2& a, const Vec2& b) {
        return a.Distance(target) > b.Distance(target);
    };
    std::priority_queue<Vec2, std::vector<Vec2>, decltype(cmp)> pq(cmp);
    
    std::unordered_map<int64_t, Vec2> parent;
    std::unordered_map<int64_t, float> cost_so_far;
    std::unordered_set<int64_t> visited;
    
    int64_t startKey = pointKey(start, searchStep);
    pq.push(start);
    visited.insert(startKey);
    parent[startKey] = start;
    cost_so_far[startKey] = 0;

    const Vec2 dirs[8] = {
        Vec2(1,1), Vec2(1,-1), Vec2(-1,1), Vec2(-1,-1),
        Vec2(1,0), Vec2(-1,0), Vec2(0,1), Vec2(0,-1)
    };

    bool found = false;
    Vec2 goalQuant;

    while (!pq.empty() && !found) {
        Vec2 cur = pq.top();
        pq.pop();
        
        for (auto d : dirs) {
            Vec2 nxt = cur.Add(d.Normalized().MulScalar(searchStep));
            
            // Limite pelo raio de busca
            if (nxt.Distance(start) > chaseRadius) continue;
            
            int64_t k = pointKey(nxt, searchStep);
            float new_cost = cost_so_far[pointKey(cur, searchStep)] + cur.Distance(nxt);
            
            // Se já visitado com custo menor, pula
            if (visited.count(k) && new_cost >= cost_so_far[k]) continue;
            
            // Teste de colisão
            if (checkCollision(nxt.x, nxt.y)) continue;
            
            visited.insert(k);
            cost_so_far[k] = new_cost;
            parent[k] = cur;
            pq.push(nxt);

            if (nxt.Distance(target) < searchStep) {
                found = true;
                goalQuant = nxt;
                break;
            }
        }
    }

    if (!found) return {};

    std::vector<Vec2> rev;
    int64_t curKey = pointKey(goalQuant, searchStep);
    int64_t startKey2 = startKey;
    while (curKey != startKey2) {
        Vec2 v = parent[curKey];
        rev.push_back(goalQuant);
        goalQuant = v;
        curKey = pointKey(goalQuant, searchStep);
    }
    std::reverse(rev.begin(), rev.end());
    rev.push_back(target);
    
    return rev;
}

void ZombieFast::NotifyCollision(GameObject& other) {
    IsoCollider* colB = (IsoCollider*) other.GetComponent("IsoCollider");
    if (colB != nullptr && !colB->passable) {
        if (other.GetComponent("HitAttack") != nullptr) {
            if (auto* hitAttack = static_cast<HitAttack*>(other.GetComponent("HitAttack"))) {
                Damage(hitAttack->GetDamage());
                return;
            }
        }

        IsoCollider* colA = (IsoCollider*) associated.GetComponent("IsoCollider");
        Rect before = colA->box;
        Rect after = Collision::Solve(colA->box, colB->box, colA->prevBox);
        Vec2 diff = (after.TopLeft() - before.TopLeft()).ToCart();

        colA->box = after;
        associated.box = associated.box.Add(diff);
    }

    if (auto* hitAttack = static_cast<HitAttack*>(other.GetComponent("HitAttack"))) {
        Damage(hitAttack->GetDamage() + 20);
    }
}

void ZombieFast::Render() {
    
}

bool ZombieFast::Is(std::string type) {
    return type == "ZombieFast";
}
