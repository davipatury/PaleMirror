#include "components/PlayerController.h"

#include "core/Game.h"

PlayerController::PlayerController(GameObject &associated) : Component(associated) {

}

void PlayerController::Update(float dt) {
    Character* chr = (Character*) associated.GetComponent("Character");
    if (!chr) return;

    float moveX = 0;
    float moveY = 0;
    if (INPUT_MANAGER.IsKeyDown(SDLK_w)) moveY--;
    if (INPUT_MANAGER.IsKeyDown(SDLK_a)) moveX--;
    if (INPUT_MANAGER.IsKeyDown(SDLK_s)) moveY++;
    if (INPUT_MANAGER.IsKeyDown(SDLK_d)) moveX++;

    if (INPUT_MANAGER.HasController()) {
        Vec2 leftAxis = INPUT_MANAGER.ControllerAxis(LEFT_JOYSTICK);
        moveX = leftAxis.x;
        moveY = leftAxis.y;
    }

    if (INPUT_MANAGER.KeyPress('c') || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_B)) {
        chr->Issue(Character::Command(Character::Command::ATTACK, 0, 0));
    }
    if (moveX != 0 || moveY != 0) {
        chr->Issue(Character::Command(Character::Command::MOVE, moveX, moveY));
    }
}

void PlayerController::Start() { }

void PlayerController::Render() { }

bool PlayerController::Is(std::string type) {
    return type == "PlayerController";
}
