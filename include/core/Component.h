#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>
#include <string>

class GameObject;

class Component
{
public:
    Component(GameObject& associated);
    virtual ~Component();

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Start() { }
    virtual void NotifyCollision(GameObject& other) { }
    virtual bool Is(std::string type) = 0;

    GameObject& associated;
};

#endif // COMPONENT_H
