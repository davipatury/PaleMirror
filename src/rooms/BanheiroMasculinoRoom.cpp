#include "rooms/BanheiroMasculinoRoom.h"

BanheiroMasculinoRoom::BanheiroMasculinoRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/banheiro.png";
    lockCameraOnRoom = false;
    basePos = Vec2{60000, 0};
    cameraLimits = Rect(basePos.x, basePos.y, 1500, 1125);

    entryPos.push_back(Vec2{60245, 718});
}

void BanheiroMasculinoRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-BANHEIRO-MASC] ColliderTL", 877, 402);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 48}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-BANHEIRO-MASC] ColliderTR", 877, 402);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-BANHEIRO-MASC] ColliderBR", 1211, 600);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 48}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-BANHEIRO-MASC] ColliderBL", 218, 800);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);
    // Lixeira
    GameObject* lixeira = createGO("[OBJ-BANHEIRO-MASC] ColliderLixeira", 485, 600);
    lixeira->box.w = 70;
    lixeira->box.h = 70;
    lixeira->AddComponent(new IsoCollider(*lixeira, {1, 1}));
    state->AddObject(lixeira);

    // Pia
    GameObject* pia = createGO("[OBJ-BANHEIRO-MASC] Pias", 561, 410);
    pia->AddComponent(new SpriteRenderer(*pia, "Recursos/img/objetos/pias.png"));
    pia->AddComponent(new IsoCollider(*pia, {0.2, 1.5}, {96, -45}));
    state->AddObject(pia);

    // Boxes
    GameObject* boxes = createGO("[OBJ-BANHEIRO-MASC] Boxes", 757, 418);
    boxes->AddComponent(new SpriteRenderer(*boxes, "Recursos/img/objetos/boxes.png"));
    boxes->AddComponent(new IsoCollider(*boxes, {0.5, 1.2}, {61, -90}));
    state->AddObject(boxes);

    // Espelho
    GameObject* espelho = createGO("[OBJ-BANHEIRO-MASC] Espelho", 560, 175);
    espelho->AddComponent(new SpriteRenderer(*espelho, "Recursos/img/objetos/espelho_banheiro.png"));
    espelho->AddComponent(new Interactable(*espelho, [this](State* state, GameObject* go) {
        GameData::playerVictory = true;
        Game::GetInstance().Push(new EndState());
    }, 250, nullptr, {100, 230}, "Interagir"));
    state->AddObject(espelho);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ--BANHEIRO-MASC] Porta", 278, 815);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 7), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-BANHEIRO-MASC] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_banheiro.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
