#ifndef ACTION_H
#define ACTION_H

#include <memory>

class GameObject;

class Action {
public:
    virtual ~Action() = default;

    virtual void Execute() = 0;
    virtual std::unique_ptr<Action> Clone() const = 0;
};

#endif