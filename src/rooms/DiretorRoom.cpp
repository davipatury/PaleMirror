#include "rooms/DiretorRoom.h"


DiretorRoom::DiretorRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_base_invertida.png";
    lockCameraOnRoom = true;
    basePos = Vec2{90000, 0};

    entryPos.push_back(Vec2{90703, 762});
}

void DiretorRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-DIRETOR] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-DIRETOR] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-DIRETOR] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-DIRETOR] ColliderBL", 410, 673);
    blGO->box.w = 25;
    blGO->box.h = 25;
    blGO->AddComponent(new IsoCollider(*blGO, {24, 1}));
    state->AddObject(blGO);

    // Estante
    GameObject* estante = createGO("[OBJ] Estante", 485+32, 590-25);
    SpriteRenderer* estanteSprite = new SpriteRenderer(*estante, "Recursos/img/objetos/estante.png");
    estanteSprite->SetScale(3.3, 3.3);
    estante->AddComponent(estanteSprite);
    estante->AddComponent(new IsoCollider(*estante, {0.35, 0.8}, {0, -17}));
    state->AddObject(estante);


    // Sofá
    GameObject* sofa = createGO("[OBJ] Sofá", 660, 529);
    SpriteRenderer* sofaSprite = new SpriteRenderer(*sofa, "Recursos/img/objetos/sofa.png");
    sofaSprite->SetScale(3.15, 3.15);
    sofa->AddComponent(sofaSprite);
    sofa->AddComponent(new IsoCollider(*sofa, {0.35, 0.8}, {16, -44}));
    state->AddObject(sofa);

    // Cadeira-Chique
    GameObject* cadeirac = createGO("[OBJ] Cadeira Chique", 880, 568);
    SpriteRenderer* cadeiracSprite = new SpriteRenderer(*cadeirac, "Recursos/img/objetos/cadeira_chique.png");
    cadeiracSprite->SetScale(1.9, 1.9);
    cadeiracSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeirac->AddComponent(cadeiracSprite);
    cadeirac->AddComponent(new IsoCollider(*cadeirac, {0.7, 0.75}, {-6, -15}));
    state->AddObject(cadeirac);

    // Mesa-Centro
    GameObject* mesac = createGO("[OBJ] Mesa Centro", 752+29, 660);
    SpriteRenderer* mesacSprite = new SpriteRenderer(*mesac, "Recursos/img/objetos/mesa_centro.png");
    mesacSprite->SetScale(2.4, 2.4);
    mesacSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesac->AddComponent(mesacSprite);
    mesac->AddComponent(new IsoCollider(*mesac, {0.85, 0.95}, {0, -53}));
    state->AddObject(mesac);

    // 29 - 29
    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 9), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-ARTES] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
