#include "rooms/Room.h"

Room::Room(State* state) {
    this->state = state;
    this->lockCameraOnRoom = false;
    this->cameraLimits = Rect(0, 0, 0, 0);
}

void Room::Build() {
    bgGO = createGO("[BG] Room", 0, 0);
    bgGO->AddComponent(new SpriteRenderer(*bgGO, bgFile));
    bgGO->box.z = -2;
    state->AddObject(bgGO);

    if (bgMusicFile) {
        bgMusic.Open(bgMusicFile);
    }
}

void Room::Enter() {
    if (bgMusic.IsOpen()) {
        bgMusic.Play();
    }

    if (lockCameraOnRoom && bgGO != nullptr) {
        Camera::Follow(bgGO);
    } else if (Character::player != nullptr) {
        Camera::Follow(&Character::player->associated);
    }

    Camera::SetCameraLimits(cameraLimits);
}

void Room::Leave() {
    if (bgMusic.IsOpen()) bgMusic.Stop();
}

GameObject* Room::createGO(std::string name, float x, float y) {
    GameObject* go = new GameObject(name);
    go->box.x = basePos.x + x;
    go->box.y = basePos.y + y;
    return go;
}
