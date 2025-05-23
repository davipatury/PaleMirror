#include "PlayerController.h"

#include "Game.h"

PlayerController::PlayerController(GameObject &associated) : Component(associated) {

}

void PlayerController::Update(float dt) {
    Character* chr = (Character*) associated.GetComponent("Character");
    float moveX = 0;
    float moveY = 0;
    if (InputManager::GetInstance().IsKeyDown(SDLK_w)) moveY--;
    if (InputManager::GetInstance().IsKeyDown(SDLK_a)) moveX--;
    if (InputManager::GetInstance().IsKeyDown(SDLK_s)) moveY++;
    if (InputManager::GetInstance().IsKeyDown(SDLK_d)) moveX++;
    if (InputManager::GetInstance().KeyPress('c')) {
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
