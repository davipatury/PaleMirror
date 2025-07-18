#include "rooms/ProfessoresRoom.h"


ProfessoresRoom::ProfessoresRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_base_invertida.png";
    lockCameraOnRoom = true;
    basePos = Vec2{100000, 0};

    entryPos.push_back(Vec2{100703, 762});
}

void ProfessoresRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-PROFESSOR] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-PROFESSOR] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-PROFESSOR] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-PROFESSOR] ColliderBL", 410, 673);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 3), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-ARTES] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
