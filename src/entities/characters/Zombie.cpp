#include "entities/characters/Zombie.h"
#include "entities/projectiles/HitAttack.h"
#include "core/Game.h"
#include "core/GameData.h"

int Zombie::zombieCounter;

Zombie::Zombie(GameObject& associated) : Component(associated), deathSound("Recursos/audio/Dead.wav"), hitSound("Recursos/audio/sounds/monster/hit1.wav"), monsterSound("Recursos/audio/sounds/monster/monstro500-2.wav") {
    hitpoints = 100;

    SpriteRenderer* sprite = new SpriteRenderer(associated,"Recursos/img/Monster/monster.png", 4, 1);
    associated.AddComponent(sprite);

    Animator* animator = new Animator(associated);
    associated.AddComponent(animator);
    animator->AddAnimation("walking", Animation(0, 3, 0.3));
    animator->AddAnimation("walkingFlip", Animation(0, 3, 0.3, SDL_FLIP_HORIZONTAL));
    animator->SetAnimation("walking");
    
    zombieCounter++;
    hit = false;
    hitTimer.Restart();
    deathTimer.Restart();
    monsterSoundTimer.Restart();
}

void Zombie::Start() {
    Collider* collider = new Collider(associated, Vec2(0.65, 1), Vec2(-5, 0));
    associated.AddComponent(collider);
    IsoCollider* isoCollider = new IsoCollider(associated, {0.5, 0.5}, {-15, 15}, false);
    associated.AddComponent(isoCollider);
}

void Zombie::Damage(int damage) {
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

        // Animação de Hit

    }
}

void Zombie::Update(float dt) {
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
        if (animator) {
            /*
            if (moveLeft) {
                animator->SetAnimation("hitLeft");
            } else {
                animator->SetAnimation("hit");
            }
            */
        }
        hit = false;
    }

    if (hitpoints > 0) {
        if (Character::player) {
            Vec2 playerPos = Character::player->Pos();
            Vec2 selfPos = associated.box.Center();
            float dist = playerPos.Distance(selfPos);

            if (dist > chaseRadius) return;

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
                anim->SetAnimation(left ? "walking" : "walkingFlip");
            }
        }
    } else if (deathTimer.Get() >= 2) {
        zombieCounter--;
        associated.RequestDelete();
    }

    

    // std::cout << "Zombie position: " << associated.box.x << ", " << associated.box.y << std::endl;
}

static inline int64_t pointKey(const Vec2& v, float step) {
    int ix = int(std::floor(v.x / step));
    int iy = int(std::floor(v.y / step));
    return (int64_t(ix) << 32) | uint32_t(iy);
}

bool Zombie::checkCollision(float x, float y) {
    auto oA = CURRENT_STATE.objectArray;
    IsoCollider* colliderB = (IsoCollider*) associated.GetComponent("IsoCollider");
    // x e y é o centro
    auto isoBox = colliderB->MakeIsoBoxForPoint(x - associated.box.w/2, y - associated.box.h/2);
    isoB = isoBox;
    
    for (int i = 0; i < oA.size(); i++) {
        GameObject* go = oA[i].get();
        if (go == &associated) continue;
        IsoCollider* colliderA = (IsoCollider*) go->GetComponent("IsoCollider");
        if(colliderA != nullptr and colliderA->box.Collides(isoBox) and (go->GetComponent("Character") == nullptr)) {
            return true;
        }
    }
    return false;
}

std::vector<Vec2> Zombie::computePath(const Vec2& target) {
    Vec2 start = associated.box.Center();

    std::queue<Vec2> q;
    std::unordered_map<int64_t, Vec2> parent;
    std::unordered_set<int64_t> visited;
    
    int64_t startKey = pointKey(start, searchStep);
    q.push(start);
    visited.insert(startKey);
    parent[startKey] = start;

    const Vec2 dirs[8] = {Vec2(1,1), Vec2(1,-1), Vec2(-1,1), Vec2(-1,-1),Vec2(1,0), Vec2(-1,0), Vec2(0,1), Vec2(0,-1)};

    bool found = false;
    Vec2 goalQuant;

    while (!q.empty() && !found) {
        Vec2 cur = q.front(); q.pop();
        for (auto d : dirs) {
            Vec2 nxt = cur.Add(d.Normalized().MulScalar(searchStep));
            // limite pelo raio de busca
            if (nxt.Distance(start) > chaseRadius) continue;
            int64_t k = pointKey(nxt, searchStep);
            if (visited.count(k)) continue;
            // teste de colisão em coord real
            if (checkCollision(nxt.x, nxt.y)) continue;
            visited.insert(k);
            parent[k] = cur;
            if (nxt.Distance(target) < 2*searchStep) {
                found = true;
                goalQuant = nxt;
                parent[pointKey(goalQuant, searchStep)] = cur;
                break;
            }
            q.push(nxt);
        }
    }
    if (!found) return {};

    // reconstrói caminho de goalQuant → start
    std::vector<Vec2> rev;
    int64_t curKey = pointKey(goalQuant, searchStep);
    int64_t startKey2 = startKey;
    while (curKey != startKey2) {
        Vec2 v = parent[curKey];
        curKey = pointKey(v, searchStep);
        if(curKey != startKey2) rev.push_back(v);
    }
    std::reverse(rev.begin(), rev.end());

    // adiciona target exato ao fim
    rev.push_back(target);
    return rev;
}

void Zombie::NotifyCollision(GameObject& other) {
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
        Damage(hitAttack->GetDamage());
    }
}

void Zombie::Render() {
    
}

bool Zombie::Is(std::string type) {
    return type == "Zombie";
}
