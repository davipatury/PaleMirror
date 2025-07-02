#include "rooms/ScienceClassRoom.h"

ScienceClassRoom::ScienceClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_ciencias.png";
    lockCameraOnRoom = true;
    basePos = Vec2{30000, 0};

    entryPos.push_back(Vec2{30703, 762});
}

void ScienceClassRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-CIENCIAS] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-CIENCIAS] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-CIENCIAS] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-CIENCIAS] ColliderBL", 410, 673);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);

    // Mesa e Cadeira
    GameObject* mesacad = createGO("[OBJ] CadeiraMesa", 700, 550);
    SpriteRenderer* mcSprite = new SpriteRenderer(*mesacad, "Recursos/img/objetos/CADEIRAeMESA.png");
    mcSprite->SetScale(1.25, 1.25);
    mcSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesacad->AddComponent(mcSprite);
    mesacad->AddComponent(new IsoCollider(*mesacad, {1, 1}, {0, -35}));
    state->AddObject(mesacad);

    // Mesa e Cadeira
    GameObject* mesacad2 = createGO("[OBJ] CadeiraMesa ESQ", 600, 610);
    SpriteRenderer* mcSprite2 = new SpriteRenderer(*mesacad2, "Recursos/img/objetos/CADEIRAeMESA.png");
    mcSprite2->SetScale(1.25, 1.25);
    mcSprite2->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesacad2->AddComponent(mcSprite2);
    mesacad2->AddComponent(new IsoCollider(*mesacad2, {1, 1}, {0, -35}));
    state->AddObject(mesacad2);

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
    std::unique_ptr<Action> backRoomAction(new ChangeRoomAction(state, "main", 1));
    Interactable* interactDoorBack = new Interactable(*roomDoorBack, std::move(backRoomAction));
    interactDoorBack->SetRequireMouseOver(true);
    interactDoorBack->SetActivationDistance(30);
    roomDoorBack->AddComponent(interactDoorBack);
    state->AddObject(roomDoorBack);
}
