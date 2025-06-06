#include "core/GameObject.h"

GameObject::GameObject() {
    isDead = false;
    started = false;
    angleDeg = 0;
}

GameObject::~GameObject() {
    components.clear();
}

void GameObject::Update(float dt) {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        cpt->Update(dt);
    }
}

void GameObject::Render() {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        cpt->Render();
    }
}

void GameObject::Start() {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        cpt->Start();
    }
    started = true;
}

void GameObject::NotifyCollision(GameObject &other) {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        cpt->NotifyCollision(other);
    }
}

bool GameObject::IsDead() {
    return isDead;
}

void GameObject::RequestDelete() {
    isDead = true;
}

void GameObject::AddComponent(Component* cpt) {
    components.emplace_back(cpt);
}

void GameObject::RemoveComponent(Component* remCpt) {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        if (cpt == remCpt) {
            components.erase(components.begin() + i);
            return;
        }
    }
}

Component* GameObject::GetComponent(std::string type) {
    for (int i = 0; i < components.size(); ++i) {
        Component* cpt = components[i].get();
        if (cpt->Is(type)) {
            return cpt;
        }
    }
    return nullptr;
}
