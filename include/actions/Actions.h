#ifndef ACTIONS_H
#define ACTIONS_H

#include <memory>
#include <functional>

#include "actions/Actions.h"
#include "math/Vec2.h"
#include "core/GameObject.h"
#include "core/State.h"
#include "states/StageState.h"

class GameObject;

class Actions {
public:
    static std::function<void (State*)> ChangeRoom(std::string targetRoom, int entryPosIndex = 0);
    static std::function<void (State*)> Document(const std::string& path);
    static std::function<void (State*)> CollectItem();
};

#endif
