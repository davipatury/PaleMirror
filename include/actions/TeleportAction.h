#ifndef TELEPORTACTION_H
#define TELEPORTACTION_H

#include "actions/Action.h"
#include "math/Vec2.h"
#include "core/GameObject.h"

class TeleportAction : public Action {
public:
    TeleportAction(const Vec2& dst, GameObject* target = nullptr, bool followTarget = false);
    void Execute() override;

private:
    Vec2 destination;
    GameObject* target;
    bool followTarget;
};

#endif
