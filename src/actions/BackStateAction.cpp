#include "actions/BackStateAction.h"
#include "core/Game.h"

BackStateAction::BackStateAction(bool* popRequested) : popRequested(popRequested) {}

void BackStateAction::Execute() {
    *popRequested = true;
} 