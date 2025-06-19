#include "states/StageState.h"
#include "actions/TeleportAction.h"
#include "actions/DocumentAction.h"
#include "components/Interactable.h"
#include <memory>
#include "math.h"
//#define DEBUG_VISIBILITY

StageState::StageState() {
    started = false;
    quitRequested = false;
    popRequested = false;

    backgroundMusic.Open("Recursos/audio/BGM.wav");
    // backgroundMusic.Play();

    // BG
    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, "Recursos/img/objetos/bg.png");
    //bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.x = 0;
    bg->box.y = 0;
    bg->box.z = -2;
    AddObject(bg);

    // Talvez criar uma funcao GenerateMap() pra gerar esses objetos pra não spammar o constructor de gameobject
    // Arvores
    GameObject* tree1 = new GameObject("[OBJ] Tree");
    tree1->AddComponent(new SpriteRenderer(*tree1, "Recursos/img/objetos/tree.png"));
    tree1->box.x = 380;
    tree1->box.y = 910;
    tree1->box.z = 0;
    AddObject(tree1);

    // Canteiros jardim
    GameObject* garden1 = new GameObject("[OBJ] Garden");
    garden1->AddComponent(new SpriteRenderer(*garden1, "Recursos/img/objetos/garden.png"));
    garden1->AddComponent(new IsoCollider(*garden1, {1.5, 0.5}, {-175, -190}));
    garden1->box.x = 1826;
    garden1->box.y = 1365;
    garden1->box.z = 0;
    AddObject(garden1);

    // Bloco de salas
    GameObject* crblock = new GameObject("[OBJ] ClassRoomBlock");
    crblock->AddComponent(new SpriteRenderer(*crblock, "Recursos/img/objetos/classroom_block.png"));
    crblock->AddComponent(new IsoCollider(*crblock, {1.5, 0.5}, {-245, -180}));
    crblock->box.x = 2321;
    crblock->box.y = 843;
    crblock->box.z = 0;
    AddObject(crblock);


    // -------------------------- ClassRoom
    GameObject* classroom = new GameObject("[BG] ClassRoom");
    SpriteRenderer* classroomSprite = new SpriteRenderer((*classroom), "Recursos/img/rooms/ClassRoom.png", 1, 1);
    classroomSprite->SetScale(0.50, 0.50);
    classroom->AddComponent(classroomSprite);
    classroom->box.x = 10000;
    classroom->box.y = 0;
    classroom->box.z = -2;
    AddObject(classroom);
    // Porta
    GameObject* door = new GameObject("[OBJ] Mirror");
    door->box.x = 2600;
    door->box.y = 1350;
    door->box.z = 0;
    auto it = new SpriteRenderer(*door, "Recursos/img/objetos/espelho.png");
    it->SetFrame(0, SDL_FLIP_HORIZONTAL);
    it->SetScale(1.5, 1.5);
    door->AddComponent(it);
    Vec2 destinoSala(10765, 855);
    std::unique_ptr<Action> teleportRoomAction(new TeleportAction(destinoSala, classroom, true));
    Interactable* interactRoom = new Interactable(*door, std::move(teleportRoomAction));
    interactRoom->SetRequireMouseOver(true);
    door->AddComponent(interactRoom);
    AddObject(door);

    // Mesa e Cadeira
    GameObject* mesacad = new GameObject("[OBJ] CadeiraMesa");
    SpriteRenderer* mcSprite = new SpriteRenderer(*mesacad, "Recursos/img/objetos/CADEIRAeMESA.png");
    mcSprite->SetScale(1.25, 1.25);
    mcSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesacad->AddComponent(mcSprite);
    mesacad->AddComponent(new IsoCollider(*mesacad, {1, 1}, {0, -35}));
    mesacad->box.x = 10700;
    mesacad->box.y = 550;
    mesacad->box.z = 0;
    AddObject(mesacad);

    // Mesa e Cadeira
    GameObject* mesacad2 = new GameObject("[OBJ] CadeiraMesa ESQ");
    SpriteRenderer* mcSprite2 = new SpriteRenderer(*mesacad2, "Recursos/img/objetos/CADEIRAeMESA.png");
    mcSprite2->SetScale(1.25, 1.25);
    mcSprite2->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesacad2->AddComponent(mcSprite2);
    mesacad2->AddComponent(new IsoCollider(*mesacad2, {1, 1}, {0, -35}));
    mesacad2->box.x = 10600;
    mesacad2->box.y = 610;
    mesacad2->box.z = 0;
    AddObject(mesacad2);

    // Cadeira
    GameObject* cadeira = new GameObject("[OBJ] CadeiraAzul");
    SpriteRenderer* cadeiraSprite = new SpriteRenderer(*cadeira, "Recursos/img/objetos/CADEIRAM.png");
    cadeiraSprite->SetScale(1.25, 1.25);
    cadeiraSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    cadeira->AddComponent(cadeiraSprite);
    cadeira->AddComponent(new IsoCollider(*cadeira, {0.7, 0.7}, {0, -17}));
    cadeira->box.x = 10700;
    cadeira->box.y = 680;
    cadeira->box.z = 0;
    AddObject(cadeira);

    // Mesa
    GameObject* mesa = new GameObject("[OBJ] MesaAzul");
    SpriteRenderer* mesaSprite = new SpriteRenderer(*mesa, "Recursos/img/objetos/MESAazul.png");
    mesaSprite->SetScale(1.25, 1.25);
    mesaSprite->SetFrame(0, SDL_FLIP_HORIZONTAL);
    mesa->AddComponent(mesaSprite);
    mesa->AddComponent(new IsoCollider(*mesa, {1, 1}, {0, -17}));

    std::unique_ptr<Action> docRoomAction(new DocumentAction("Recursos/img/objetos/carteira-azul-icon.jpg"));
    Interactable* interactdocRoom = new Interactable(*mesa, std::move(docRoomAction));
    interactdocRoom->SetRequireMouseOver(true);
    interactdocRoom->SetActivationDistance(60);
    mesa->AddComponent(interactdocRoom);
    mesa->box.x = 10713;
    mesa->box.y = 690;
    mesa->box.z = 0;
    AddObject(mesa);

    // Porta
    GameObject* roomDoorBack = new GameObject("[OBJ] Porta");
    roomDoorBack->box.x = 10765;
    roomDoorBack->box.y = 855;
    roomDoorBack->box.z = 0;
    Vec2 destinoDoorBack(2606, 1480);
    std::unique_ptr<Action> teleportDoorBackAction(new TeleportAction(destinoDoorBack));
    Interactable* interactDoorBack = new Interactable(*roomDoorBack, std::move(teleportDoorBackAction));
    interactDoorBack->SetRequireMouseOver(true);
    interactDoorBack->SetActivationDistance(25);
    roomDoorBack->AddComponent(interactDoorBack);
    AddObject(roomDoorBack);
    

    // TileMap
    /*
    GameObject* map = new GameObject();
    TileSet* tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
    TileMap* tileMap = new TileMap((*map), "Recursos/map/map.txt", tileSet);
    map->AddComponent(tileMap);
    map->box.x = 0;
    map->box.y = 0;
    map->box.z = -1;
    AddObject(map);
    */

    // Player
    GameObject* character = new GameObject("[PLAYER]");
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    character->AddComponent(new PlayerController(*character));
    character->AddComponent(charCmp);
    character->box.x = 2500;
    character->box.y = 1450;
    character->box.z = 0;
    Character::player = charCmp;
    Camera::Follow(character);
    AddObject(character);

    // WaveSpawner
    /*GameObject* waveSpawner = new GameObject();
    WaveSpawner* waveSpawnerCmp = new WaveSpawner(*waveSpawner);
    waveSpawner->AddComponent(waveSpawnerCmp);
    AddObject(waveSpawner);*/

    // HP HUD
    /*GameObject* hud = new GameObject();
    HealthHUD* hpHud = new HealthHUD(*hud);
    hud->AddComponent(hpHud);
    hud->box.z = 3;
    AddObject(hud);*/
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {}

void StageState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void StageState::Update(float dt) {
    // Quit requested
    if (InputManager::GetInstance().QuitRequested()) {
        quitRequested = true;
    }

    if (InputManager::GetInstance().IsKeyDown(ESCAPE_KEY)) {
        popRequested = true;
        InputManager::GetInstance().ReleaseKey(ESCAPE_KEY);
    }

    // Debug
    if (InputManager::GetInstance().IsKeyDown(SDLK_1)) {
        for (int i = 0; i < objectArray.size(); i++) {
            GameObject* go = objectArray[i].get();
            if (go->GetComponent("Zombie") != nullptr || go->GetComponent("AIController") != nullptr) {
                go->RequestDelete();
            }
        }
    }

    // Update camera
    Camera::Update(dt);

    // Spawn test block
    if (InputManager::GetInstance().KeyPress(' ')) {
        // teste object
        GameObject* test = new GameObject();
        SpriteRenderer* testSprite = new SpriteRenderer((*test), "Recursos/img/torre.png", 1, 1);
        IsoCollider* isoCol = new IsoCollider(*test, {1, 1}, {0, -23});
        ShadowCaster* shadow = new ShadowCaster(*test, {0, -23});
        //test->AddComponent(shadow);
        test->AddComponent(testSprite);
        test->AddComponent(isoCol);
        test->box.x = InputManager::GetInstance().GetMouseX() + Camera::pos.x - (test->box.w / 2);;
        test->box.y = InputManager::GetInstance().GetMouseY() + Camera::pos.y - (test->box.h / 2);;
        test->box.z = 0;
        AddObject(test);
    }

    // Spawn mirror puzzle
    if (InputManager::GetInstance().KeyPress('m')) {
        GameObject* mp = new GameObject();
        mp->AddComponent(new MirrorPuzzle((*mp), std::vector<MirrorPuzzle::Piece>{
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/1.png", Vec2{0, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/2.png", Vec2{81, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/3.png", Vec2{0, 198}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/4.png", Vec2{57, 148}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/5.png", Vec2{0, 290}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/6.png", Vec2{184, 382})
        }));
        mp->box.z = 2;
        AddObject(mp);
    }

    // Update game objects
    UpdateArray(dt);

    // Collision
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        Collider* colliderA = (Collider*) go->GetComponent("Collider");
        if (colliderA != nullptr) {
            for (int j = i + 1; j < objectArray.size(); j++) {
                GameObject* go2 = objectArray[j].get();
                Collider* colliderB = (Collider*) go2->GetComponent("Collider");
                if (colliderB != nullptr) {
                    if (Collision::IsColliding(
                            colliderA->box,
                            colliderB->box,
                            go->angleDeg,
                            go2->angleDeg
                            )) {
                        go->NotifyCollision(*go2);
                        go2->NotifyCollision(*go);
                    }
                }
            }
        }
    }

    // Iso collision
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        IsoCollider* colliderA = (IsoCollider*) go->GetComponent("IsoCollider");
        if (colliderA != nullptr) {
            for (int j = i + 1; j < objectArray.size(); j++) {
                GameObject* go2 = objectArray[j].get();
                IsoCollider* colliderB = (IsoCollider*) go2->GetComponent("IsoCollider");
                if (colliderB != nullptr) {
                    if (Collision::IsColliding(
                            colliderA->box,
                            colliderB->box,
                            go->angleDeg,
                            go2->angleDeg
                            )) {
                        go->NotifyCollision(*go2);
                        go2->NotifyCollision(*go);
                    }
                }
            }
        }
    }

    // Remove dead game objects
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        if (go->IsDead()) {
            objectArray.erase(objectArray.begin() + i);
        }
    }

    // End game
    if (Character::player == nullptr) {
        popRequested = true;
        GameData::playerVictory = false;
        Game::GetInstance().Push(new EndState());
    } else if (WaveSpawner::allWavesEnded) {
        popRequested = true;
        GameData::playerVictory = true;
        Game::GetInstance().Push(new EndState());
    }
}

void StageState::Render() {
    RenderArray();
/*

    Vec2 origin = IsoRect(Character::player->associated.box).Center() - Camera::pos;

    // Darkness
    SDL_Texture* light = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1200, 900);
    SDL_SetRenderTarget(GAME_RENDERER, light);
    SDL_SetTextureBlendMode(light, SDL_BLENDMODE_BLEND);

    SDL_Rect screenRect;
    screenRect.x = screenRect.y = 0;
    screenRect.w = 1200;
    screenRect.h = 900;

    SDL_Texture* backlight = Resources::GetImage("Recursos/img/backlight.png");
    SDL_RenderCopy(GAME_RENDERER, backlight, &screenRect, &screenRect);

    // Flashlight
    Vec2 mousePos = Vec2(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    float mouseAngle = mousePos.Angle(origin) + M_PI * 0.5;
    int flashlightSize = 600;
    float flashlightAngle = M_PI / 8;
    Vec2 flLeft = Vec2(origin.x + flashlightSize * sin(mouseAngle - flashlightAngle), origin.y - flashlightSize * cos(mouseAngle - flashlightAngle));
    Vec2 flRight = Vec2(origin.x + flashlightSize * sin(mouseAngle + flashlightAngle), origin.y - flashlightSize * cos(mouseAngle + flashlightAngle));

    std::vector<SDL_Vertex> lightVertices = {
        SDL_Vertex {SDL_FPoint {origin.x, origin.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 0.5, 0.5 } },
        SDL_Vertex {SDL_FPoint {flLeft.x, flLeft.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 1, 0.3 } },
        SDL_Vertex {SDL_FPoint {flRight.x, flRight.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 1, 0.7 } }
    };
    SDL_BlendMode bm;
    SDL_GetRenderDrawBlendMode(GAME_RENDERER, &bm);
    SDL_Texture* flashlight = Resources::GetImage("Recursos/img/flashlight.png");
    SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDOPERATION_MAXIMUM, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_MINIMUM);
    SDL_SetTextureBlendMode(flashlight, blendMode);
    int r = SDL_RenderGeometry(GAME_RENDERER, flashlight, lightVertices.data(), lightVertices.size(), nullptr, 0);

    SDL_SetRenderTarget(GAME_RENDERER, nullptr);
    SDL_RenderCopy(GAME_RENDERER, light, &screenRect, &screenRect);
*/
}

void StageState::Pause() {
    pauseX = Character::player->Pos().x;
    pauseY = Character::player->Pos().y;

}
void StageState::Resume() {
    Camera::Unfollow();
    Camera::pos.x = 0;
    Camera::pos.y = 0;

    GameObject* character = new GameObject();
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    character->AddComponent(charCmp);
    PlayerController* playerController = new PlayerController(*character);
    character->AddComponent(playerController);
    character->box.x = pauseX;
    character->box.y = pauseY;
    character->box.z = 0;
    Character::player = charCmp;
    Camera::Follow(character);
    AddObject(character);
}
