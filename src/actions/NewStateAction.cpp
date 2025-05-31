#include "actions/NewStateAction.h"
#include "core/Game.h"

NewStateAction::NewStateAction(State* newState) : newState(newState) {}

void NewStateAction::Execute() {
    Game::GetInstance().Push(newState);
} 