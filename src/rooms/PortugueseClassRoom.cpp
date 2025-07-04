#include "rooms/PortugueseClassRoom.h"

PortugueseClassRoom::PortugueseClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_portugues.png";
    lockCameraOnRoom = true;
    basePos = Vec2{40000, 0};

    entryPos.push_back(Vec2{40703, 762});
}

void PortugueseClassRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-PORTUGUES] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-PORTUGUES] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-PORTUGUES] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-PORTUGUES] ColliderBL", 410, 673);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    std::unique_ptr<Action> backRoomAction(new ChangeRoomAction(state, "main", 0));
    Interactable* interactDoorBack = new Interactable(*roomDoorBack, std::move(backRoomAction));
    interactDoorBack->SetRequireMouseOver(true);
    interactDoorBack->SetActivationDistance(30);
    roomDoorBack->AddComponent(interactDoorBack);
    state->AddObject(roomDoorBack);

    GameObject* zombie = createGO("[Monster]", 675, 500);
    zombie->AddComponent(new Zombie(*zombie));
    state->AddObject(zombie);
}
