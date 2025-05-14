#include <State.h>

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
        go->Update(dt);
    }
}

void State::RenderArray() {
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        go->Render();
    }
}

bool State::PopRequested() {
    return popRequested;
}

bool State::QuitRequested() {
    return quitRequested;
}
