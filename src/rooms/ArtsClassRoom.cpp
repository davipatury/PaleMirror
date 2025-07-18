#include "rooms/ArtsClassRoom.h"

#include "components/puzzles/PipePuzzle.h"
#include "components/puzzles/PaintPuzzle.h"

ArtsClassRoom::ArtsClassRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_artes_invertida.png";
    lockCameraOnRoom = true;
    basePos = Vec2{20000, 0};

    entryPos.push_back(Vec2{20703, 762});
}

void ArtsClassRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-ARTES] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-ARTES] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-ARTES] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-ARTES] ColliderBL", 410, 673);
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

    // Velas
    GameObject* vela = createGO("[OBJ] Vela", 851, 685);
    vela->AddComponent(new SpriteRenderer(*vela, "Recursos/img/items/vela.png"));
    vela->AddComponent(new Interactable(*vela, Actions::CollectItem(ITEM_VELA), ITEM_COLLECT_DIST, nullptr, {-35, -35}, "Pegar"));
    state->AddObject(vela);

    GameObject* vela2 = createGO("[OBJ] Vela", 851, 615);
    vela2->AddComponent(new SpriteRenderer(*vela2, "Recursos/img/items/vela.png"));
    vela2->AddComponent(new Interactable(*vela2, Actions::CollectItem(ITEM_VELA), ITEM_COLLECT_DIST, nullptr, {-35, -35}, "Pegar"));
    state->AddObject(vela2);

    // Cavalete - PaintPuzzle
    GameObject* cavalete = createGO("[OBJ] Cavalete", 875, 636);
    SpriteRenderer* cavaleteSprite = new SpriteRenderer(*cavalete, "Recursos/img/objetos/cavalete.png");
    cavaleteSprite->SetScale(2, 2);
    cavalete->AddComponent(cavaleteSprite);
    cavalete->AddComponent(new IsoCollider(*cavalete, {0.5, 0.5}, {-17, -17}));
    cavalete->AddComponent(new Interactable(*cavalete, nullptr));
    cavalete->AddComponent(new PaintPuzzle::Initiator(*cavalete));
    state->AddObject(cavalete);

    // Caixa Fusivel - FusePuzzle
    GameObject* caixaFusivel = createGO("[OBJ] Caixa Fusivel", 458, 544);
    SpriteRenderer* caixaFusivelSprite = new SpriteRenderer(*caixaFusivel, "Recursos/img/objetos/caixa_energia.png", 2, 1);
    caixaFusivelSprite->SetScale(1.25, 1.25);
    caixaFusivel->AddComponent(caixaFusivelSprite);
    caixaFusivel->AddComponent(new Interactable(*caixaFusivel, nullptr));
    caixaFusivel->AddComponent(new LightEmitter(*caixaFusivel, {
        // Offset, Scale, Enabled, Sprite path
        {{31, 35}, {0.25, 0.25}, false, "Recursos/img/lighting/backlight_inv_yellow.png"}
    }));
    caixaFusivel->AddComponent(new FusePuzzle::Initiator(*caixaFusivel));
    state->AddObject(caixaFusivel);

    // Cano parede - PipePuzzle
    GameObject* canoParede = createGO("[OBJ] Cano Parede", 668, 456);
    SpriteRenderer* canoParedeSprite = new SpriteRenderer(*canoParede, "Recursos/img/objetos/cano_parede.png");
    canoParedeSprite->SetScale(1.5, 1.5);
    canoParede->AddComponent(canoParedeSprite);
    canoParede->AddComponent(new Interactable(*canoParede, nullptr));
    canoParede->AddComponent(new PipePuzzle::Initiator(*canoParede));
    state->AddObject(canoParede);

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
