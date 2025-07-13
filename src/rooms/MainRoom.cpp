#include "rooms/MainRoom.h"

MainRoom::MainRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/objetos/bg.png";
    bgMusicFile = "Recursos/audio/Mapa.mp3";
    lockCameraOnRoom = false;
    basePos = Vec2{0, 0};

    cameraLimits = Rect(basePos.x, basePos.y, 4500, 3375);

    // Entry CRB 1
    entryPos.push_back(Vec2{934, 2390});
    entryPos.push_back(Vec2{1193, 2536});

    // Entry CRB 2
    entryPos.push_back(Vec2{1902, 1837});
    entryPos.push_back(Vec2{2156, 1986});

    // Entry CRB 3
    entryPos.push_back(Vec2{2902, 1262});
    entryPos.push_back(Vec2{3157, 1410});

    // Entry bathroom block
    entryPos.push_back(Vec2{1336, 1549});
    entryPos.push_back(Vec2{1579, 1403});
}

void MainRoom::Build() {
    Room::Build();

    GameObject* zombie = createGO("[Monster]", 1000, 1300);
    zombie->AddComponent(new Zombie(*zombie));
    state->AddObject(zombie);

    // Arvores
    GameObject* tree1 = createGO("[OBJ] Tree1", 540, 1299);
    tree1->AddComponent(new SpriteRenderer(*tree1, "Recursos/img/objetos/tree.png"));
    tree1->AddComponent(new IsoCollider(*tree1, {0.6, 0.8}, {-37, -51}));
    state->AddObject(tree1);

    GameObject* tree2 = createGO("[OBJ] Tree2", 1821, 573);
    tree2->AddComponent(new SpriteRenderer(*tree2, "Recursos/img/objetos/tree.png"));
    tree2->AddComponent(new IsoCollider(*tree2, {0.6, 0.8}, {-37, -51}));
    state->AddObject(tree2);

    // Canteiros jardim
    GameObject* garden1 = createGO("[OBJ] Garden1", 1368, 1749);
    garden1->AddComponent(new SpriteRenderer(*garden1, "Recursos/img/objetos/garden.png"));
    garden1->AddComponent(new IsoCollider(*garden1, {1.2, 0.6}, {-160, -130}));
    state->AddObject(garden1);

    GameObject* garden2 = createGO("[OBJ] Garden2", 2336, 1190);
    garden2->AddComponent(new SpriteRenderer(*garden2, "Recursos/img/objetos/garden.png"));
    garden2->AddComponent(new IsoCollider(*garden2, {1.2, 0.6}, {-160, -130}));
    state->AddObject(garden2);

    // Bloco de salas 1
    GameObject* crblock1 = createGO("[OBJ] ClassRoomBlock1", 807, 1889);
    crblock1->AddComponent(new SpriteRenderer(*crblock1, "Recursos/img/objetos/classroom_block.png"));
    crblock1->AddComponent(new IsoCollider(*crblock1, {1.1, 0.54}, {-185, -160}, false, true));
    crblock1->AddComponent(new ShadowCaster(*crblock1));
    state->AddObject(crblock1);

    // Bloco de salas 1 - Porta 1
    GameObject* door11 = createGO("[OBJ] Porta1-1", crblock1->box.x + 181, crblock1->box.y + 421);
    auto door11sr = new SpriteRenderer(*door11, "Recursos/img/objetos/porta_highlight.png");
    door11->AddComponent(door11sr);
    door11->AddComponent(new IsoCollider(*door11, {0.5, 0.4}, {-20, -22}));
    std::unique_ptr<Action> changeRoomDoor11(new ChangeRoomAction(state, "portuguese"));
    Interactable* interactDoor11 = new Interactable(*door11, std::move(changeRoomDoor11), door11sr);
    interactDoor11->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    door11->AddComponent(interactDoor11);
    state->AddObject(door11);

    // Bloco de salas 1 - Porta 2
    GameObject* door12 = createGO("[OBJ] Porta1-2", crblock1->box.x + 433, crblock1->box.y + 564);
    auto door12sr = new SpriteRenderer(*door12, "Recursos/img/objetos/porta_highlight.png");
    door12->AddComponent(door12sr);
    door12->AddComponent(new IsoCollider(*door12, {0.5, 0.4}, {-20, -20}));
    std::unique_ptr<Action> changeRoomDoor12(new ChangeRoomAction(state, "science"));
    Interactable* interactDoor12 = new Interactable(*door12, std::move(changeRoomDoor12), door12sr);
    interactDoor12->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    door12->AddComponent(interactDoor12);
    state->AddObject(door12);

    // Bloco de salas 2
    GameObject* crblock2 = createGO("[OBJ] ClassRoomBlock2", 1771, 1337);
    crblock2->AddComponent(new SpriteRenderer(*crblock2, "Recursos/img/objetos/classroom_block.png"));
    crblock2->AddComponent(new IsoCollider(*crblock2, {1.1, 0.54}, {-185, -160}, false, true));
    //crblock2->AddComponent(new ShadowCaster(*crblock2, std::vector<Vec2>{{271, 329}, {755, 610}, {643, 671}, {514, 747}, {487, 734}, {486, 718}, {450, 698}, {437, 696}, {198, 554}, {187, 561}, {25, 471}}));
    crblock2->AddComponent(new ShadowCaster(*crblock2));
    state->AddObject(crblock2);

    // Bloco de salas 2 - Porta 1
    GameObject* door21 = createGO("[OBJ] Porta2-1", crblock2->box.x + 181, crblock2->box.y + 421);
    auto door21sr = new SpriteRenderer(*door21, "Recursos/img/objetos/porta_highlight.png");
    door21->AddComponent(door21sr);
    door21->AddComponent(new IsoCollider(*door21, {0.5, 0.4}, {-20, -22}));
    std::unique_ptr<Action> changeRoomDoor21(new ChangeRoomAction(state, "history"));
    Interactable* interactDoor21 = new Interactable(*door21, std::move(changeRoomDoor21), door21sr);
    interactDoor21->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    door21->AddComponent(interactDoor21);
    state->AddObject(door21);

    // Bloco de salas 2 - Porta 2
    GameObject* door22 = createGO("[OBJ] Porta2-2", crblock2->box.x + 433, crblock2->box.y + 564);
    auto door22sr = new SpriteRenderer(*door22, "Recursos/img/objetos/porta_highlight.png");
    door22->AddComponent(door22sr);
    door22->AddComponent(new IsoCollider(*door22, {0.5, 0.4}, {-20, -20}));
    std::unique_ptr<Action> changeRoomDoor22(new ChangeRoomAction(state, "arts"));
    Interactable* interactDoor22 = new Interactable(*door22, std::move(changeRoomDoor22), door22sr);
    interactDoor22->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    door22->AddComponent(interactDoor22);
    state->AddObject(door22);

    // Bloco de salas 3
    GameObject* crblock3 = createGO("[OBJ] ClassRoomBlock3", 2771, 762);
    crblock3->AddComponent(new SpriteRenderer(*crblock3, "Recursos/img/objetos/classroom_block.png"));
    crblock3->AddComponent(new IsoCollider(*crblock3, {1.1, 0.54}, {-185, -160}, false, true));
    crblock3->AddComponent(new ShadowCaster(*crblock3));
    state->AddObject(crblock3);

    // Bloco de banheiros
    GameObject* bathblock = createGO("[OBJ] BathRoomBlock", 1055, 908);
    bathblock->AddComponent(new SpriteRenderer(*bathblock, "Recursos/img/objetos/bathroom_block.png"));
    bathblock->AddComponent(new IsoCollider(*bathblock, {0.54, 1.1}, {71, -166}, false, true));
    bathblock->AddComponent(new ShadowCaster(*bathblock));
    state->AddObject(bathblock);

    // Bloco de banheiros - Porta feminina
    GameObject* doorBanFem = createGO("[OBJ] PortaBanheiroFem", bathblock->box.x + 293, bathblock->box.y + 561);
    auto doorBanFemSR = new SpriteRenderer(*doorBanFem, "Recursos/img/objetos/porta_highlight.png");
    doorBanFemSR->sprite.SetFlip(SDL_FLIP_HORIZONTAL);
    doorBanFem->AddComponent(doorBanFemSR);
    doorBanFem->AddComponent(new IsoCollider(*doorBanFem, {0.5, 0.4}, {-12, -18}));
    std::unique_ptr<Action> changeRoomdoorBanFem(new ChangeRoomAction(state, "banheiroFem"));
    Interactable* interactdoorBanFem = new Interactable(*doorBanFem, std::move(changeRoomdoorBanFem), doorBanFemSR);
    interactdoorBanFem->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    doorBanFem->AddComponent(interactdoorBanFem);
    state->AddObject(doorBanFem);

    // Bloco de banheiros - Porta masculino
    GameObject* doorBanMasc = createGO("[OBJ] PortaBanheiroMasc", bathblock->box.x + 536, bathblock->box.y + 420);
    auto doorBanMascSR = new SpriteRenderer(*doorBanMasc, "Recursos/img/objetos/porta_highlight.png");
    doorBanMascSR->sprite.SetFlip(SDL_FLIP_HORIZONTAL);
    doorBanMasc->AddComponent(doorBanMascSR);
    doorBanMasc->AddComponent(new IsoCollider(*doorBanMasc, {0.5, 0.4}, {-12, -18}));
    std::unique_ptr<Action> changeRoomdoorBanMasc(new ChangeRoomAction(state, "banheiroMasc"));
    Interactable* interactdoorBanMasc = new Interactable(*doorBanMasc, std::move(changeRoomdoorBanMasc), doorBanMascSR);
    interactdoorBanMasc->SetActivationDistance(DOOR_INTERACTION_DISTANCE);
    doorBanMasc->AddComponent(interactdoorBanMasc);
    state->AddObject(doorBanMasc);
}
