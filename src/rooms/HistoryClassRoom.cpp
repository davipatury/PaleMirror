#include "rooms/HistoryClassRoom.h"

HistoryClassRoom::HistoryClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_historia_invertida.png";
    lockCameraOnRoom = true;
    basePos = Vec2{10000, 0};

    entryPos.push_back(Vec2{10703, 762});
}

void HistoryClassRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-HISTORIA] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-HISTORIA] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-HISTORIA] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-HISTORIA] ColliderBL", 410, 673);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);

    // Cadeira
    GameObject* cadeira = createGO("[OBJ] CadeiraAzul", 700, 680);
    SpriteRenderer* cadeiraSprite = new SpriteRenderer(*cadeira, "Recursos/img/objetos/CADEIRAM.png");
    cadeiraSprite->SetScale(1.25, 1.25);
    cadeiraSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeira->AddComponent(cadeiraSprite);
    cadeira->AddComponent(new IsoCollider(*cadeira, {0.7, 0.7}, {0, -17}));
    state->AddObject(cadeira);

    // Mesa
    GameObject* mesa = createGO("[OBJ] MesaAzul", 713, 690);
    SpriteRenderer* mesaSprite = new SpriteRenderer(*mesa, "Recursos/img/objetos/MESAazul.png");
    mesaSprite->SetScale(1.25, 1.25);
    mesaSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesa->AddComponent(mesaSprite);
    mesa->AddComponent(new IsoCollider(*mesa, {1, 1}, {0, -17}));
    state->AddObject(mesa);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 2), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-HISTORIA] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
