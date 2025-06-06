#include "components/AIController.h"

int AIController::npcCounter;
AIController::AIController(GameObject& associated) : Component(associated) {
    state = RESTING;
    npcCounter++;
}

AIController::~AIController() {
    npcCounter--;
}

void AIController::Update(float dt) {
    Character* chr = (Character*) associated.GetComponent("Character");
    if (chr == nullptr || Character::player == nullptr) return;

    if (state == RESTING) {
        restTimer.Update(dt);
        if (restTimer.Get() >= 2) {
            state = MOVING;
        }
    } else if (state == MOVING) {
        destination = Character::player->Pos();
        if (associated.box.Center().Distance(destination) <= 200) {
            chr->Issue(Character::Command(Character::Command::SHOOT, destination.x, destination.y));
            state = RESTING;
            restTimer.Restart();
        } else {
            destination = destination.Sub(associated.box.Center());
            chr->Issue(Character::Command(Character::Command::MOVE, destination.x, destination.y));
        }
    }
}

void AIController::Render() {}

bool AIController::Is(std::string type) {
    return type == "AIController";
}
