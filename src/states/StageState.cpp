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

    // Player
    GameObject* character = new GameObject("[PLAYER]");
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    character->AddComponent(new PlayerController(*character));
    character->AddComponent(charCmp);
    character->box.x = 1639;
    character->box.y = 1656;
    character->box.z = 0;
    Character::player = charCmp;
    Camera::Follow(character);
    AddObject(character);

    GameObject* zombie = new GameObject("[Monster]");
    Zombie* zomb = new Zombie((*zombie));
    zombie->AddComponent(zomb);
    zombie->box.x = 1639;
    zombie->box.y = 1656;
    zombie->box.z = 0;
    AddObject(zombie);

    GameObject* hp = new GameObject("[HealthHUD]");
    HealthHUD* hudhp = new HealthHUD((*hp));
    hp->AddComponent(hudhp);
    hp->box.z = 5;
    AddObject(hp);
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {
    MainRoom* mainRoom = new MainRoom(this);
    mainRoom->Build();

    HistoryClassRoom* historyRoom = new HistoryClassRoom(this);
    historyRoom->Build();

    ArtsClassRoom* artsRoom = new ArtsClassRoom(this);
    artsRoom->Build();

    ScienceClassRoom* scienceRoom = new ScienceClassRoom(this);
    scienceRoom->Build();

    PortugueseClassRoom* portugueseRoom = new PortugueseClassRoom(this);
    portugueseRoom->Build();

    rooms["main"] = mainRoom;
    rooms["history"] = historyRoom;
    rooms["arts"] = artsRoom;
    rooms["science"] = scienceRoom;
    rooms["portuguese"] = portugueseRoom;

    currentRoom = mainRoom;
    mainRoom->Enter();
}

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

    if (!openUI && InputManager::GetInstance().IsKeyDown(ESCAPE_KEY)) {
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
    if (!openUI && InputManager::GetInstance().KeyPress('m')) {
        GameObject* mp = new GameObject();
        mp->AddComponent(new MirrorPuzzle((*mp), std::vector<MirrorPuzzle::Piece>{
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/1.png", Vec2{0, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/2.png", Vec2{81, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/3.png", Vec2{0, 198}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/4.png", Vec2{57, 148}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/5.png", Vec2{0, 290}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/6.png", Vec2{184, 382})
        }));
        mp->box.z = 10;
        mp->lazyRender = false;
        mp->pauseOnOpenUI = false;
        AddObject(mp);
    }

    // Spawn fuse puzzle
    if (!openUI && InputManager::GetInstance().KeyPress('f')) {
        GameObject* fp = new GameObject();
        fp->AddComponent(new FusePuzzle((*fp)));
        fp->box.z = 10;
        fp->lazyRender = false;
        fp->pauseOnOpenUI = false;
        AddObject(fp);
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

    // Update camera
    Camera::Update(dt);

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

void StageState::ChangeRoom(std::string room) {
    currentRoom->Leave();
    currentRoom = rooms[room];
    currentRoom->Enter();
}

Room* StageState::GetRoom(std::string room) {
    return rooms[room];
}
