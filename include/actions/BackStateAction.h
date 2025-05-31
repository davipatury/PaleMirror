#ifndef BACKSTATEACTION_H
#define BACKSTATEACTION_H

#include "actions/Action.h"

class BackStateAction : public Action {
public:
    BackStateAction(bool* popRequested);
    void Execute() override;

private:
    bool* popRequested;
};

#endif // BACKSTATEACTION_H 