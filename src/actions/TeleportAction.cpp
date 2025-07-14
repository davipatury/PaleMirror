#include "actions/TeleportAction.h"
#include "entities/characters/Character.h"
#include "core/GameObject.h"
#include "components/Camera.h"

TeleportAction::TeleportAction(const Vec2& dst, GameObject* target, bool followTarget)
    : destination(dst), target(target), followTarget(followTarget) {}

void TeleportAction::Execute() {
    GameObject* objectToFollow = followTarget ? target : &Character::player->associated;
    GameObject& playerGO = Character::player->associated;

    float halfW = playerGO.box.w / 2.0f;
    float halfH = playerGO.box.h / 2.0f;

    Vec2 dest = {destination.x - halfW, destination.y - halfH};
    Character::player->Issue(Character::Command(Character::Command::TELEPORT, dest.x, dest.y));

    Camera::Follow(objectToFollow);
}
