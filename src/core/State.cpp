#include "core/State.h"

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
            go->Update(dt);
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

std::vector<GameObject*> State::RenderSort() {
    std::vector<GameObject*> objs;
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        if (!go->lazyRender || Camera::PosRect().Collides(go->box)) {
            objs.push_back(go);
        }
    }

    auto zySort = [&](GameObject* a, GameObject* b) {
        if (a->box.z != b->box.z) return a->box.z < b->box.z;

        // Iso sorting
        IsoCollider* colliderA = (IsoCollider*) a->GetComponent("IsoCollider");
        IsoCollider* colliderB = (IsoCollider*) b->GetComponent("IsoCollider");
        if (colliderA != nullptr && colliderB != nullptr) {
            return colliderA->box.TopLeft().y < colliderB->box.BottomLeft().y && colliderA->box.TopLeft().x < colliderB->box.TopRight().x;
        }

        return (a->box.BottomLeft().y) < (b->box.BottomLeft().y);
    };

    std::stable_sort(objs.begin(), objs.end(), zySort);

    return objs;
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
}
