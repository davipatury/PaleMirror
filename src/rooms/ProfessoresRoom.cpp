#include "rooms/ProfessoresRoom.h"


ProfessoresRoom::ProfessoresRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/sala_base_invertida.png";
    lockCameraOnRoom = true;
    basePos = Vec2{100000, 0};

    entryPos.push_back(Vec2{100703, 762});
}

void ProfessoresRoom::Build() {
    Room::Build();

    // Boundary colliders
    // Top left
    GameObject* tlGO = createGO("[OBJ-PROFESSOR] ColliderTL", 750, 473);
    tlGO->box.w = 25;
    tlGO->box.h = 25;
    tlGO->AddComponent(new IsoCollider(*tlGO, {1, 24}));
    state->AddObject(tlGO);
    // Top right
    GameObject* trGO = createGO("[OBJ-PROFESSOR] ColliderTR", 753, 473);
    trGO->box.w = 25;
    trGO->box.h = 25;
    trGO->AddComponent(new IsoCollider(*trGO, {24, 1}));
    state->AddObject(trGO);
    // Bottom right
    GameObject* brGO = createGO("[OBJ-PROFESSOR] ColliderBR", 1090, 668);
    brGO->box.w = 25;
    brGO->box.h = 25;
    brGO->AddComponent(new IsoCollider(*brGO, {1, 24}));
    state->AddObject(brGO);
    // Bottom left
    GameObject* blGO = createGO("[OBJ-PROFESSOR] ColliderBL", 410, 673);
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

    // Estante
    GameObject* estante2 = createGO("[OBJ] Estante", 630+32, 520-25);
    SpriteRenderer* estante2Sprite = new SpriteRenderer(*estante2, "Recursos/img/objetos/estante.png");
    estante2Sprite->SetScale(3.3, 3.3);
    estante2->AddComponent(estante2Sprite);
    estante2->AddComponent(new IsoCollider(*estante2, {0.35, 0.8}, {0, -17}));
    state->AddObject(estante2);

    // Sofa
    GameObject* cadeirac = createGO("[OBJ] sofa", 880, 568);
    SpriteRenderer* cadeiracSprite = new SpriteRenderer(*cadeirac, "Recursos/img/objetos/sofa.png");
    cadeiracSprite->SetScale(3.3, 3.3);
    cadeiracSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeirac->AddComponent(cadeiracSprite);
    cadeirac->AddComponent(new IsoCollider(*cadeirac, {0.95, 0.4}, {-59, -63}));
    state->AddObject(cadeirac);

    // Mesa
    GameObject* mesac = createGO("[OBJ] Mesinha", 670, 640);
    SpriteRenderer* mesacSprite = new SpriteRenderer(*mesac, "Recursos/img/objetos/mesa_professor.png");
    mesacSprite->SetScale(2, 2);
    mesacSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesac->AddComponent(mesacSprite);
    mesac->AddComponent(new IsoCollider(*mesac, {0.5, 1}, {10, -21}));
    state->AddObject(mesac);

    // Cadeira prof
    GameObject* cadeirinha = createGO("[OBJ] Mesinha", 700, 660);
    SpriteRenderer* cadeirinhaSprite = new SpriteRenderer(*cadeirinha, "Recursos/img/objetos/cadeira_rodinha2.png");
    cadeirinhaSprite->SetScale(2, 2);
    cadeirinha->AddComponent(cadeirinhaSprite);
    cadeirinha->AddComponent(new IsoCollider(*cadeirinha, {0.35, 0.4}, {-11, -3}));
    state->AddObject(cadeirinha);

    // Livro
    GameObject* livro = createGO("[OBJ] Mesinha", 600, 600);
    SpriteRenderer* livroSprite = new SpriteRenderer(*livro, "Recursos/img/objetos/livro_verde.png");
    livroSprite->SetScale(1, 1);
    livroSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    livro->AddComponent(livroSprite);
    livro->AddComponent(new IsoCollider(*livro, {0.5, 1}, {8, -24}));
    state->AddObject(livro);

    // Porta
    GameObject* roomDoorBack = createGO("[OBJ] Porta", 765, 855);
    roomDoorBack->AddComponent(new Interactable(*roomDoorBack, Actions::ChangeRoom("main", 10), DOOR_BACK_INTERACT_DIST, nullptr, {-35, -35}, "Sair"));
    state->AddObject(roomDoorBack);

    // Filtro
    GameObject* filtro = createGO("[OBJ-ARTES] Filtro", 0, 0);
    filtro->AddComponent(new SpriteRenderer(*filtro, "Recursos/img/rooms/filtro_sala.png"));
    filtro->box.z = 1;
    state->AddObject(filtro);
}
