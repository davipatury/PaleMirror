#ifndef NEWSTATEACTION_H
#define NEWSTATEACTION_H

#include "actions/Action.h"
#include "core/State.h"

class NewStateAction : public Action {
public:
    NewStateAction(State* newState);
    void Execute() override;

private:
    State* newState;
};

#endif // NEWSTATEACTION_H 