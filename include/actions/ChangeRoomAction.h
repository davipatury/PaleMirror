#ifndef CHANGEROOMACTION_H
#define CHANGEROOMACTION_H

#include "actions/Action.h"
#include "math/Vec2.h"
#include "core/GameObject.h"
#include "core/State.h"
#include "states/StageState.h"

class ChangeRoomAction : public Action {
public:
    ChangeRoomAction(State* state, std::string targetRoom, int entryPosIndex = 0);
    void Execute() override;
private:
    std::string targetRoom;
    State* state;
    int entryPosIndex;
};

#endif // CHANGEROOMACTION_H
