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
    GameObject(std::string name = "");
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
    std::string name;

    // Do not call Render when off-screen
    bool lazyRender = true;
    // Do not call Update when off-screen
    bool lazyUpdate = false;

    bool pauseOnOpenUI = true;
private:
    std::vector<std::unique_ptr<Component>> components;
    bool isDead;
    bool started;
};

#endif // GAMEOBJECT_H
