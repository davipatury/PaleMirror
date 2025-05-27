#include "StageState.h"

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
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, "Recursos/img/obj/bg.png");
    //bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.x = 0;
    bg->box.y = 0;
    bg->box.z = -2;
    AddObject(bg);

    // Talvez criar uma funcao GenerateMap() pra gerar esses objetos pra não spammar o constructor de gameobject
    // Arvores
    GameObject* tree1 = new GameObject();
    tree1->AddComponent(new SpriteRenderer(*tree1, "Recursos/img/obj/tree.png"));
    tree1->box.x = 380;
    tree1->box.y = 910;
    tree1->box.z = 0;
    AddObject(tree1);

    // Canteiros jardim
    GameObject* garden1 = new GameObject();
    garden1->AddComponent(new SpriteRenderer(*garden1, "Recursos/img/obj/garden.png"));
    garden1->AddComponent(new IsoCollider(*garden1, {1.5, 0.5}, {-175, -190}));
    garden1->box.x = 1826;
    garden1->box.y = 1365;
    garden1->box.z = 0;
    AddObject(garden1);

    // Bloco de salas
    GameObject* crblock = new GameObject();
    crblock->AddComponent(new SpriteRenderer(*crblock, "Recursos/img/obj/classroom_block.png"));
    crblock->AddComponent(new IsoCollider(*crblock, {1.5, 0.5}, {-245, -180}));
    crblock->box.x = 2321;
    crblock->box.y = 843;
    crblock->box.z = 0;
    AddObject(crblock);

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
    GameObject* character = new GameObject();
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    PlayerController* playerController = new PlayerController(*character);
    character->AddComponent(playerController);
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

bool ZYsorting(std::shared_ptr<GameObject> a, std::shared_ptr<GameObject> b) {
    if (a->box.z == b->box.z) return (a->box.y + a->box.h) < (b->box.y + b->box.h);
    return a->box.z < b->box.z;
}

void StageState::Render() {
    sort(objectArray.begin(), objectArray.end(), ZYsorting);
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

void StageState::Pause() {}
void StageState::Resume() {}
