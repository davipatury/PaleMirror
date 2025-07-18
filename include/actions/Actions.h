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
    static void Document(const std::string& path);
    static std::function<void (State*, GameObject*)> ChangeRoom(std::string targetRoom, int entryPosIndex = 0);
    static std::function<void (State*, GameObject*)> CollectItem(std::string itemName, int amount = 1, bool deleteSelf = true);
    static std::function<void (State*, GameObject*)> OpenDialog(std::string dialogueKey, std::function<void ()> dialogueEndFunc = nullptr);
};

#endif
