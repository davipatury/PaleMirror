#include "rooms/MainRoom.h"

#include "components/puzzles/LockPuzzle.h"

MainRoom::MainRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/rooms/escola.png";
    bgMusicFile = "Recursos/audio/Mapa.mp3";
    lockCameraOnRoom = false;
    isOut = true;
    basePos = Vec2{0, 0};

    cameraLimits = Rect(basePos.x, basePos.y, 4500, 3375);

    // Entry CRB 1
    entryPos.push_back(Vec2{934, 2390});  // 0
    entryPos.push_back(Vec2{1193, 2536}); // 1

    // Entry CRB 2
    entryPos.push_back(Vec2{1902, 1837}); //2
    entryPos.push_back(Vec2{2156, 1986}); //3

    // Entry CRB 3
    entryPos.push_back(Vec2{2902, 1262}); //4
    entryPos.push_back(Vec2{3157, 1410}); //5

    // Entry bathroom block
    entryPos.push_back(Vec2{1336, 1549}); //6
    entryPos.push_back(Vec2{1579, 1403}); //7

    entryPos.push_back(Vec2{2257, 2500}); //8
}

void MainRoom::Build() {
    Room::Build();

    GameObject* zombie = createGO("[MonsterSlow]", 1000, 1300);
    zombie->AddComponent(new Zombie(*zombie));
    state->AddObject(zombie);

    /*
    GameObject* zombief = createGO("[MonsterFast]", 1200, 1600);
    zombief->AddComponent(new ZombieFast(*zombief));
    state->AddObject(zombief);
    */

    // Muro e grades
    GameObject* muro = createGO("[OBJ] MuroBR", 1905, 1620);
    muro->AddComponent(new SpriteRenderer(*muro, "Recursos/img/objetos/muro.png"));
    muro->AddComponent(new IsoCollider(*muro, {0.025, 1.75}, {1102, -615}));
    muro->AddComponent(new ShadowCaster(*muro, {0, 0}, true));
    state->AddObject(muro);
    GameObject* gradeTl = createGO("[OBJ] GradeTL", 0, 99);
    gradeTl->AddComponent(new SpriteRenderer(*gradeTl, "Recursos/img/objetos/grade_tl.png"));
    gradeTl->AddComponent(new IsoCollider(*gradeTl, {0.025, 1.8}, {1072, -644}));
    state->AddObject(gradeTl);
    GameObject* gradeTr = createGO("[OBJ] GradeTR", 2531, 120);
    gradeTr->AddComponent(new SpriteRenderer(*gradeTr, "Recursos/img/objetos/grade_tr.png"));
    gradeTr->AddComponent(new IsoCollider(*gradeTr, {1.8, 0.025}, {-1138, -496}));
    state->AddObject(gradeTr);
    GameObject* gradeBl = createGO("[OBJ] GradeBL", 0, 2042);
    gradeBl->AddComponent(new SpriteRenderer(*gradeBl, "Recursos/img/objetos/grade_bl.png"));
    gradeBl->AddComponent(new IsoCollider(*gradeBl, {1.75, 0.1}, {-1121, -426}));
    state->AddObject(gradeBl);

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

    // Estatua
    GameObject* estatua = createGO("[OBJ] Estatua", 3393, 1389);
    estatua->AddComponent(new SpriteRenderer(*estatua, "Recursos/img/objetos/estatua_diretor.png"));
    estatua->AddComponent(new IsoCollider(*estatua, {0.85, 0.87}, {-37, -120}, false, true));
    estatua->AddComponent(new ShadowCaster(*estatua));
    state->AddObject(estatua);

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
    door11->AddComponent(new Interactable(*door11, Actions::ChangeRoom("portuguese"), DOOR_INTERACT_DIST, door11sr));
    state->AddObject(door11);

    // Bloco de salas 1 - Porta 2
    GameObject* door12 = createGO("[OBJ] Porta1-2", crblock1->box.x + 433, crblock1->box.y + 564);
    auto door12sr = new SpriteRenderer(*door12, "Recursos/img/objetos/porta_highlight.png");
    door12->AddComponent(door12sr);
    door12->AddComponent(new IsoCollider(*door12, {0.5, 0.4}, {-20, -20}));
    door12->AddComponent(new Interactable(*door12, Actions::ChangeRoom("science"), DOOR_INTERACT_DIST, door12sr));
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
    door21->AddComponent(new Interactable(*door21, Actions::ChangeRoom("history"), DOOR_INTERACT_DIST, door21sr));
    state->AddObject(door21);

    // Bloco de salas 2 - Porta 2
    GameObject* door22 = createGO("[OBJ] Porta2-2", crblock2->box.x + 433, crblock2->box.y + 564);
    auto door22sr = new SpriteRenderer(*door22, "Recursos/img/objetos/porta_highlight.png");
    door22->AddComponent(door22sr);
    door22->AddComponent(new IsoCollider(*door22, {0.5, 0.4}, {-20, -20}));
    door22->AddComponent(new Interactable(*door22, Actions::ChangeRoom("arts"), DOOR_INTERACT_DIST, door22sr));
    state->AddObject(door22);

    // Bloco de salas 3
    GameObject* crblock3 = createGO("[OBJ] ClassRoomBlock3", 2771, 762);
    crblock3->AddComponent(new SpriteRenderer(*crblock3, "Recursos/img/objetos/classroom_block.png"));
    crblock3->AddComponent(new IsoCollider(*crblock3, {1.1, 0.54}, {-185, -160}, false, true));
    crblock3->AddComponent(new ShadowCaster(*crblock3));
    state->AddObject(crblock3);

    // Bloco de salas 3 - Porta 1
    GameObject* door31 = createGO("[OBJ] Porta3-1", crblock3->box.x + 181, crblock3->box.y + 431);
    auto door31sr = new SpriteRenderer(*door31, "Recursos/img/objetos/porta_highlight.png");
    door31->AddComponent(door31sr);
    door31->AddComponent(new IsoCollider(*door31, {0.5, 0.4}, {-20, -22}));
    door31->AddComponent(new Interactable(*door31, Actions::ChangeRoom("diretor"), DOOR_INTERACT_DIST, door31sr));
    state->AddObject(door31);

    // Bloco de salas 3 - Porta 2
    GameObject* door32 = createGO("[OBJ] Porta3-2", crblock3->box.x + 433, crblock3->box.y + 564);
    auto door32sr = new SpriteRenderer(*door32, "Recursos/img/objetos/porta_highlight.png");
    door32->AddComponent(door32sr);
    door32->AddComponent(new IsoCollider(*door32, {0.5, 0.4}, {-20, -22}));
    door32->AddComponent(new Interactable(*door32, Actions::ChangeRoom("professores"), DOOR_INTERACT_DIST, door32sr));
    state->AddObject(door32);

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
    doorBanFem->AddComponent(new Interactable(*doorBanFem, Actions::ChangeRoom("banheiroFem"), DOOR_INTERACT_DIST, doorBanFemSR));
    state->AddObject(doorBanFem);

    // Bloco de banheiros - Porta masculino
    GameObject* doorBanMasc = createGO("[OBJ] PortaBanheiroMasc", bathblock->box.x + 536, bathblock->box.y + 420);
    auto doorBanMascSR = new SpriteRenderer(*doorBanMasc, "Recursos/img/objetos/porta_highlight.png");
    doorBanMascSR->sprite.SetFlip(SDL_FLIP_HORIZONTAL);
    doorBanMasc->AddComponent(doorBanMascSR);
    doorBanMasc->AddComponent(new LockPuzzle::Initiator(*doorBanMasc, "1234", "banheiroMasc"));
    doorBanMasc->AddComponent(new IsoCollider(*doorBanMasc, {0.5, 0.4}, {-12, -18}));
    doorBanMasc->AddComponent(new Interactable(*doorBanMasc, Actions::ChangeRoom("banheiroMasc"), DOOR_INTERACT_DIST, doorBanMascSR));
    state->AddObject(doorBanMasc);

    // Caixa Fusivel - FusePuzzle
    GameObject* caixaFusivel = createGO("[OBJ] Caixa Fusivel", 1211, 1505);
    SpriteRenderer* caixaFusivelSprite = new SpriteRenderer(*caixaFusivel, "Recursos/img/objetos/caixa_energia.png", 2, 1);
    caixaFusivelSprite->SetScale(1.25, 1.25);
    caixaFusivel->AddComponent(caixaFusivelSprite);
    caixaFusivel->AddComponent(new IsoCollider(*caixaFusivel, {1, 1}, {0, 0}));
    caixaFusivel->AddComponent(new Interactable(*caixaFusivel, nullptr));
    caixaFusivel->AddComponent(new LightEmitter(*caixaFusivel, {
        // Offset, Scale, Enabled, Sprite path
        {{41, 35}, {0.25, 0.25}, true, "Recursos/img/lighting/backlight_inv_yellow.png"}
    }));
    caixaFusivel->AddComponent(new FusePuzzle::Initiator(*caixaFusivel));
    state->AddObject(caixaFusivel);
}
