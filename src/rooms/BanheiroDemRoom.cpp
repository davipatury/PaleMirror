#include "rooms/BanheiroDemRoom.h"
#include "hud/FlashlightHUD.h"

BanheiroDemRoom::BanheiroDemRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/banheiro.png";
    lockCameraOnRoom = false;
    basePos = Vec2{80000, 0};
    cameraLimits = Rect(basePos.x, basePos.y, 1500, 1125);
    entryPos.push_back(Vec2{80500, 718});
}

void BanheiroDemRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-BANHEIRO-DEM] ColliderTL", 877, 402);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 48}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-BANHEIRO-DEM] ColliderTR", 877, 402);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-BANHEIRO-DEM] ColliderBR", 1211, 600);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 48}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-BANHEIRO-DEM] ColliderBL", 218, 800);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);
    // Lixeira
    GameObject* lixeira = createGO("[OBJ-BANHEIRO-DEM] ColliderLixeira", 485, 600);
    lixeira->box.w = 70;
    lixeira->box.h = 70;
    lixeira->AddComponent(new IsoCollider(*lixeira, {1, 1}));
    state->AddObject(lixeira);
    // Pia
    GameObject* pia = createGO("[OBJ-BANHEIRO-DEM] Pias", 561, 410);
    pia->AddComponent(new SpriteRenderer(*pia, "Recursos/img/objetos/pias.png"));
    pia->AddComponent(new IsoCollider(*pia, {0.2, 1.5}, {96, -45}));
    state->AddObject(pia);

    // Boxes
    GameObject* boxes = createGO("[OBJ-BANHEIRO-DEM] Boxes", 757, 418);
    boxes->AddComponent(new SpriteRenderer(*boxes, "Recursos/img/objetos/boxesfeminino.png"));
    boxes->AddComponent(new IsoCollider(*boxes, {0.5, 1.2}, {61, -90}));
    state->AddObject(boxes);

    // Simbolo ritual
    /*GameObject* ritual = createGO("[OBJ-BANHEIRO-DEM] Ritual", 433, 730);
    SpriteRenderer* ritualSR = new SpriteRenderer(*ritual, "Recursos/img/objetos/simbolo_ritual_1.png");
    ritualSR->SetColorMod(255, 82, 0);
    ritual->AddComponent(ritualSR);
    state->AddObject(ritual);*/

    // Ritual
    GameObject* ritualD = createGO("[OBJ-BANHEIRO-DEM] Ritual", 433, 730);
    ritualD->AddComponent(new SpriteRenderer(*ritualD, "Recursos/img/objetos/simbolo_ritual_2.png", 3, 1));
    ritualD->AddComponent(new Interactable(*ritualD, nullptr));
    ritualD->AddComponent(new LightEmitter(*ritualD, {
                                                       // Offset, Scale, Enabled, Sprite path
                                                       {{26, -5}, {0.5, 0.5}, false, "Recursos/img/lighting/backlight_inv_orange.png"},
                                                       {{232, 112}, {0.5, 0.5}, false, "Recursos/img/lighting/backlight_inv_orange.png"}
                                                   }));
    ritualD->AddComponent(new RunaRitual(*ritualD, true));
    state->AddObject(ritualD);

    // Espelho
    GameObject* espelho = createGO("[OBJ-BANHEIRO-DEM] Espelho", 560, 175);
    espelho->AddComponent(new SpriteRenderer(*espelho, "Recursos/img/objetos/espelho_banheiro-helena.png"));
    espelho->AddComponent(new Interactable(*espelho, nullptr));
    espelho->AddComponent(new MirrorzinhoPuzzle::Initiator(*espelho));
    state->AddObject(espelho);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ--BANHEIRO-DEM] Porta", 278, 815);
    state->AddObject(roomDoorBack);
}
