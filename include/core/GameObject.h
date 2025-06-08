#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "core/Component.h"
#include "math/Rect.h"

class GameObject
{
public:
    GameObject();
    ~GameObject();

    void Update(float dt);
    void Render();
    void Start();
    bool IsDead();
    void RequestDelete();
    void AddComponent(Component* cpt);
    void RemoveComponent(Component* cpt);
    Component* GetComponent(std::string type);

    void NotifyCollision(GameObject& other);

    Rect box;
    float angleDeg;

    // Do not call Render when off-screen
    bool lazyRender = true;
    // Do not call Update when off-screen
    bool lazyUpdate = false;
private:
    std::vector<std::unique_ptr<Component>> components;
    bool isDead;
    bool started;
};

#endif // GAMEOBJECT_H
