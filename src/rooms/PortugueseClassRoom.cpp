#include "rooms/PortugueseClassRoom.h"

PortugueseClassRoom::PortugueseClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_portugues_invertida.png";
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
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 0), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    GameObject* zombie = createGO("[Monster]", 675, 500);
    zombie->AddComponent(new Zombie(*zombie));
    state->AddObject(zombie);

    GameObject* vela2 = createGO("[OBJ] Vela", 851, 615);
    vela2->AddComponent(new SpriteRenderer(*vela2, "Recursos/img/items/vela.png"));
    vela2->AddComponent(new Interactable(*vela2, Actions::CollectItem(ITEM_VELA), ITEM_COLLECT_DIST, nullptr, {-35, -35}, "Pegar"));
    state->AddObject(vela2);

    // Filtrods
    GameObject* filtro = createGO("[OBJ-PORTUGUES] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
