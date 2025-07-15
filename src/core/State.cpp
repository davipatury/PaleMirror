#include "core/State.h"
#include "math/Line.h"
#include "components/SpriteRenderer.h"
#include "physics/Collision.h"
#include <unordered_map>
#include <set>
#include <map>
#include <queue>

using namespace std;

const float PI = acos(-1);

State::State() {
    popRequested = false;
    quitRequested = false;
    started = false;
}

State::~State() {
    objectArray.clear();
}

std::weak_ptr<GameObject> State::AddObject(GameObject* go) {
    std::shared_ptr<GameObject> ptr(go);
    objectArray.push_back(ptr);
    if (started) ptr->Start();
    std::weak_ptr<GameObject> weak(ptr);
    return weak;
}

std::weak_ptr<GameObject> State::GetObjectPtr(GameObject* go) {
    for (int i = 0; i < objectArray.size(); ++i) {
        GameObject* lgo = objectArray[i].get();
        if (lgo == go) {
            std::weak_ptr<GameObject> weak(objectArray[i]);
            return weak;
        }
    }
    std::weak_ptr<GameObject> weak;
    return weak;
}

void State::StartArray() {
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        go->Start();
    }
}

void State::UpdateArray(float dt) {
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        if (!go->lazyUpdate || Camera::PosRect().Collides(go->box)) {
            if (!(openUI && go->pauseOnOpenUI)) {
                go->Update(dt);
            }
        }
    }
}

void State::RenderArray() {
    std::vector<GameObject*> objs = RenderSort();
    for (int i = 0; i < objs.size(); i++) {
        GameObject* go = objs[i];
        go->Render();
    }
}

bool State::dependsOn(GameObject* A, GameObject* B) {
    // Iso sorting
    IsoCollider* colliderA = (IsoCollider*) A->GetComponent("IsoCollider");
    IsoCollider* colliderB = (IsoCollider*) B->GetComponent("IsoCollider");
    if (colliderA != nullptr && colliderB != nullptr) {
        //return !(colliderA->box.TopLeft().y < colliderB->box.BottomLeft().y && colliderA->box.TopLeft().x < colliderB->box.TopRight().x);
        return !(colliderA->box.TopLeft().y - colliderB->box.BottomLeft().y < -0.01f && colliderA->box.TopLeft().x - colliderB->box.TopRight().x < -0.01f);
    }
    return !(A->box.BottomLeft().y) < (B->box.BottomLeft().y);
}

std::vector<GameObject*> State::RenderSort(int z) {
    std::vector<GameObject*> objs;
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        if ((z == -1 || go->box.z == z) && (!go->lazyRender || Camera::PosRect().Collides(go->box))) {
            objs.push_back(go);
        }
    }
    vector<int> inDegree((int)objs.size());
    unordered_map<int, vector<int>> adj;

    map<int, vector<int>> zlist;

    for(int i=0; i<objs.size(); i++){
        zlist[objs[i]->box.z].push_back(i);
        for(int j=i+1; j<objs.size(); j++){
            if(objs[i]->box.z != objs[j]->box.z) continue;
            //SpriteRenderer* ca = (SpriteRenderer*) objs[i]->GetComponent("SpriteRenderer");
            //SpriteRenderer* cb = (SpriteRenderer*) objs[j]->GetComponent("SpriteRenderer");
            //float angle1 = objs[i]->box.Center().Angle(objs[j]->box.Center());
            //float angle2 = objs[j]->box.Center().Angle(objs[i]->box.Center());
            if (Collision::IsColliding(objs[i]->box, objs[j]->box, 0, 0)) {
                /*
                    std::cout << "Colisão detectada entre: " << std::endl;
                    std::cout << "Objeto 1: " << (objs[i]->name.empty() ? "Sem nome" : objs[i]->name) << std::endl;
                    std::cout << "Objeto 2: " << (objs[j]->name.empty() ? "Sem nome" : objs[j]->name) << std::endl;
                    */
                if(dependsOn(objs[i], objs[j])){
                    //std::cout << "Dependência: " << objs[i]->name << " depende de " << objs[j]->name << std::endl;
                    adj[j].push_back(i);
                    inDegree[i]++;
                }else{
                    //std::cout << "Dependência: " << objs[j]->name << " depende de " << objs[i]->name << std::endl;
                    adj[i].push_back(j);
                    inDegree[j]++;
                }
                //std::cout << "------------------------" << std::endl;
            }
        }
    }

    // Resolve um Toposort pra cada Z
    vector<GameObject*> GOSorted;
    for(auto [zval, vt]: zlist){
        queue<int> q;
        int sz = (int)vt.size();
        for(int i=0; i<sz; i++){
            if(!inDegree[vt[i]]) q.push(vt[i]);
        }
        vector<GameObject*> sorted;
        sorted.reserve(sz);
        set<int> st;
        while(!q.empty()){
            int u = q.front(); q.pop();
            sorted.push_back(objs[u]);
            st.insert(u);
            for(auto v: adj[u]){
                inDegree[v]--;
                if(!inDegree[v]) q.push(v);
            }
        }
        // Se tiver ciclo, só chama em qualquer ordem
        if((int)sorted.size() < sz){
            for(int i=0; i<sz; i++){
                if(!st.count(vt[i])) sorted.push_back(objs[vt[i]]);
            }
        }
        GOSorted.insert(GOSorted.end(), sorted.begin(), sorted.end());
    }
    return GOSorted;
}

bool State::PopRequested() {
    return popRequested;
}

void State::RequestPop() {
    popRequested = true;
}

bool State::QuitRequested() {
    return quitRequested;
}
