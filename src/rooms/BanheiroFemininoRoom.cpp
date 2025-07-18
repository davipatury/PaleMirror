#include "rooms/BanheiroFemininoRoom.h"

#include "components/puzzles/MirrorPuzzle.h"

BanheiroFemininoRoom::BanheiroFemininoRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/banheiro.png";
    lockCameraOnRoom = false;
    basePos = Vec2{50000, 0};
    cameraLimits = Rect(basePos.x, basePos.y, 1500, 1125);

    entryPos.push_back(Vec2{50245, 718});
    entryPos.push_back(Vec2{50782, 487});
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
    ritual->AddComponent(new SpriteRenderer(*ritual, "Recursos/img/objetos/simbolo_ritual.png", 3, 1));
    ritual->AddComponent(new Interactable(*ritual, nullptr));
    ritual->AddComponent(new LightEmitter(*ritual, {
        // Offset, Scale, Enabled, Sprite path
        {{26, -5}, {0.5, 0.5}, false, "Recursos/img/lighting/backlight_inv_orange.png"},
        {{232, 112}, {0.5, 0.5}, false, "Recursos/img/lighting/backlight_inv_orange.png"}
    }));
    ritual->AddComponent(new RunaRitual(*ritual));
    state->AddObject(ritual);

    // Marias
    /*GameObject* marias = createGO("[OBJ-BANHEIRO-INTRO] Marias", 310, 700);
    marias->AddComponent(new SpriteRenderer(*marias, "Recursos/img/objetos/marias.png"));
    marias->AddComponent(new IsoCollider(*marias, {1.0, 1.0}, {-20, -30}));
    state->AddObject(marias);
    */

    // Espelho
    GameObject* espelho = createGO("[OBJ-BANHEIRO-FEM] Espelho", 560, 175);
    espelho->AddComponent(new SpriteRenderer(*espelho, "Recursos/img/objetos/espelho_banheiro.png"));
    espelho->AddComponent(new Interactable(*espelho, nullptr));
    espelho->AddComponent(new MirrorPuzzle::Initiator(*espelho));
    state->AddObject(espelho);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ--BANHEIRO-FEM] Porta", 278, 815);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 6), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-BANHEIRO-FEM] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_banheiro.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
