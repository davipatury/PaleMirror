#include "rooms/MainRoom.h"

MainRoom::MainRoom(State* state) : Room(state) {
    bgFile = "Recursos/img/objetos/bg.png";
    //bgMusicFile = "Recursos/audio/BGM.wav";
    lockCameraOnRoom = false;
    basePos = Vec2{0, 0};

    // Entry 0
    entryPos.push_back(Vec2{2606, 1480});
}

void MainRoom::Build() {
    Room::Build();

    // Arvores
    GameObject* tree1 = createGO("[OBJ] Tree", 380, 910);
    tree1->AddComponent(new SpriteRenderer(*tree1, "Recursos/img/objetos/tree.png"));
    state->AddObject(tree1);

    // Canteiros jardim
    GameObject* garden1 = createGO("[OBJ] Garden", 1826, 1365);
    garden1->AddComponent(new SpriteRenderer(*garden1, "Recursos/img/objetos/garden.png"));
    garden1->AddComponent(new IsoCollider(*garden1, {1.5, 0.5}, {-175, -190}));
    state->AddObject(garden1);

    // Bloco de salas
    GameObject* crblock = createGO("[OBJ] ClassRoomBlock", 2321, 843);
    crblock->AddComponent(new SpriteRenderer(*crblock, "Recursos/img/objetos/classroom_block.png"));
    crblock->AddComponent(new IsoCollider(*crblock, {1.5, 0.5}, {-245, -180}));
    state->AddObject(crblock);

    // Porta
    GameObject* door = createGO("[OBJ] Mirror", 2600, 1350);
    auto it = new SpriteRenderer(*door, "Recursos/img/objetos/espelho.png");
    it->SetFrame(0, SDL_FLIP_HORIZONTAL);
    it->SetScale(1.5, 1.5);
    door->AddComponent(it);
    std::unique_ptr<Action> changeRoomAction(new ChangeRoomAction(state, "classroom"));
    Interactable* interactRoom = new Interactable(*door, std::move(changeRoomAction));
    interactRoom->SetRequireMouseOver(true);
    door->AddComponent(interactRoom);
    state->AddObject(door);
}
