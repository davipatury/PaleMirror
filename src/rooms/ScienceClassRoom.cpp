#include "rooms/ScienceClassRoom.h"

ScienceClassRoom::ScienceClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_ciencias_invertida.png";
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

    // Cadeira
    GameObject* cadeira11 = createGO("[OBJ] CadeiraAzul", 696, 676);
    SpriteRenderer* cadeira11Sprite = new SpriteRenderer(*cadeira11, "Recursos/img/objetos/CADEIRAM.png");
    cadeira11Sprite->SetScale(1.65, 1.65);
    cadeira11Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeira11->AddComponent(cadeira11Sprite);
    cadeira11->AddComponent(new IsoCollider(*cadeira11, {0.7, 0.7}, {0, -17}));
    state->AddObject(cadeira11);

    // Mesa
    GameObject* mesa11 = createGO("[OBJ] MesaAzul", 696+36, 676+11);
    SpriteRenderer* mesa11Sprite = new SpriteRenderer(*mesa11, "Recursos/img/objetos/MESAazul.png");
    mesa11Sprite->SetScale(1.65, 1.65);
    mesa11Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesa11->AddComponent(mesa11Sprite);
    mesa11->AddComponent(new IsoCollider(*mesa11, {0.8, 1}, {0, -17}));
    state->AddObject(mesa11);

    // Cadeira
    GameObject* cadeira12 = createGO("[OBJ] CadeiraAzul", 566, 596);
    SpriteRenderer* cadeira12Sprite = new SpriteRenderer(*cadeira12, "Recursos/img/objetos/CADEIRAM.png");
    cadeira12Sprite->SetScale(1.65, 1.65);
    cadeira12Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeira12->AddComponent(cadeira12Sprite);
    cadeira12->AddComponent(new IsoCollider(*cadeira12, {0.7, 0.7}, {0, -17}));
    state->AddObject(cadeira12);

    // Mesa
    GameObject* mesa12 = createGO("[OBJ] MesaAzul", 566+36, 598+12);
    SpriteRenderer* mesa12Sprite = new SpriteRenderer(*mesa12, "Recursos/img/objetos/MESAazul.png");
    mesa12Sprite->SetScale(1.65, 1.65);
    mesa12Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesa12->AddComponent(mesa12Sprite);
    mesa12->AddComponent(new IsoCollider(*mesa12, {0.8, 1}, {0, -17}));
    state->AddObject(mesa12);

    // Cadeira
    GameObject* cadeira21 = createGO("[OBJ] CadeiraAzul", 690, 495);
    SpriteRenderer* cadeira21Sprite = new SpriteRenderer(*cadeira21, "Recursos/img/objetos/CADEIRAM.png");
    cadeira21Sprite->SetScale(1.65, 1.65);
    cadeira21Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeira21->AddComponent(cadeira21Sprite);
    cadeira21->AddComponent(new IsoCollider(*cadeira21, {0.7, 0.7}, {0, -17}));
    state->AddObject(cadeira21);

    // Mesa
    GameObject* mesa21 = createGO("[OBJ] MesaAzul", 690+36, 495+21);
    SpriteRenderer* mesa21Sprite = new SpriteRenderer(*mesa21, "Recursos/img/objetos/MESAazul.png");
    mesa21Sprite->SetScale(1.65, 1.65);
    mesa21Sprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesa21->AddComponent(mesa21Sprite);
    mesa21->AddComponent(new IsoCollider(*mesa21, {0.8, 1}, {0, -17}));
    state->AddObject(mesa21);

    // Mesa
    GameObject* mesaprof = createGO("[OBJ] MesaProfessor", 900, 600);
    SpriteRenderer* mesaprofSprite = new SpriteRenderer(*mesaprof, "Recursos/img/objetos/mesa_professor.png");
    mesaprofSprite->SetScale(1.65, 1.65);
    mesaprofSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesaprof->AddComponent(mesaprofSprite);
    mesaprof->AddComponent(new IsoCollider(*mesaprof, {0.8, 1}, {0, -17}));
    state->AddObject(mesaprof);

    // Cadeira
    GameObject* cadeiraprof = createGO("[OBJ] CadeiraProfessor", 900+35, 600+10);
    SpriteRenderer* cadeiraprofSprite = new SpriteRenderer(*cadeiraprof, "Recursos/img/objetos/cadeira_rodinhas.png");
    cadeiraprofSprite->SetScale(1.65, 1.65);
    cadeiraprofSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeiraprof->AddComponent(cadeiraprofSprite);
    cadeiraprof->AddComponent(new IsoCollider(*cadeiraprof, {0.8, 1}, {0, -17}));
    state->AddObject(cadeiraprof);

    GameObject* lanterna = createGO("[OBJ] Vela", 851, 615);
    lanterna->AddComponent(new SpriteRenderer(*lanterna, "Recursos/img/items/lanterna.png"));
    lanterna->AddComponent(new Interactable(*lanterna, Actions::CollectItem(ITEM_LANTERNA), ITEM_COLLECT_DIST, nullptr, {-35, -35}, "Pegar"));
    state->AddObject(lanterna);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 1), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-CIENCIAS] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
