#include "rooms/BanheiroFemininoRoom.h"

BanheiroFemininoRoom::BanheiroFemininoRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/banheiro.png";
    lockCameraOnRoom = false;
    basePos = Vec2{50000, 0};
    cameraLimits = Rect(basePos.x, basePos.y, 1500, 1125);

    entryPos.push_back(Vec2{50245, 718});
}

void BanheiroFemininoRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-BANHEIRO-FEM] ColliderTL", 877, 402);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 48}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-BANHEIRO-FEM] ColliderTR", 877, 402);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-BANHEIRO-FEM] ColliderBR", 1211, 600);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 48}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-BANHEIRO-FEM] ColliderBL", 218, 800);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);
    // Lixeira
    GameObject* lixeira = createGO("[OBJ-BANHEIRO-FEM] ColliderLixeira", 485, 600);
    lixeira->box.w = 70;
    lixeira->box.h = 70;
    lixeira->AddComponent(new IsoCollider(*lixeira, {1, 1}));
    state->AddObject(lixeira);
    // Pia
    GameObject* pia = createGO("[OBJ-BANHEIRO-FEM] Pias", 561, 410);
    pia->AddComponent(new SpriteRenderer(*pia, "Recursos/img/objetos/pias.png"));
    pia->AddComponent(new IsoCollider(*pia, {0.2, 1.5}, {96, -45}));
    state->AddObject(pia);

    // Boxes
    GameObject* boxes = createGO("[OBJ-BANHEIRO-FEM] Boxes", 757, 418);
    boxes->AddComponent(new SpriteRenderer(*boxes, "Recursos/img/objetos/boxesfeminino.png"));
    boxes->AddComponent(new IsoCollider(*boxes, {0.5, 1.2}, {61, -90}));
    state->AddObject(boxes);

    // Simbolo ritual
    GameObject* ritual = createGO("[OBJ-BANHEIRO-FEM] Ritual", 433, 730);
    SpriteRenderer* ritualSR = new SpriteRenderer(*ritual, "Recursos/img/objetos/simbolo_ritual.png");
    ritualSR->SetColorMod(255, 82, 0);
    ritual->AddComponent(ritualSR);
    state->AddObject(ritual);

    // Espelho
    GameObject* espelho = createGO("[OBJ-BANHEIRO-FEM] Espelho", 560, 175);
    espelho->AddComponent(new SpriteRenderer(*espelho, "Recursos/img/objetos/espelho_banheiro.png"));
    state->AddObject(espelho);

    // Filtro invertido
    /*
    GameObject* filtro = createGO("[OBJ-BANHEIRO-FEM] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_banheiro.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);*/

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ--BANHEIRO-FEM] Porta", 278, 815);
    std::unique_ptr<Action> backRoomAction(new ChangeRoomAction(state, "main", 6));
    Interactable* interactDoorBack = new Interactable(*roomDoorBack, std::move(backRoomAction));
    interactDoorBack->SetActivationDistance(30);
    roomDoorBack->AddComponent(interactDoorBack);
    state->AddObject(roomDoorBack);
}
