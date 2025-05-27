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
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/Background.png", 1, 1);
    bgSprite->SetCameraFollower(true);
    bg->AddComponent(bgSprite);
    bg->box.z = -2;
    AddObject(bg);

    // TileMap
    GameObject* map = new GameObject();
    TileSet* tileSet = new TileSet(64, 64, "Recursos/img/Tileset.png");
    TileMap* tileMap = new TileMap((*map), "Recursos/map/map.txt", tileSet);
    map->AddComponent(tileMap);
    map->box.x = 0;
    map->box.y = 0;
    map->box.z = -1;
    AddObject(map);

    // Player
    GameObject* character = new GameObject();
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    PlayerController* playerController = new PlayerController(*character);
    character->AddComponent(playerController);
    character->AddComponent(charCmp);
    character->box.x = 1280;
    character->box.y = 1280;
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
    GameObject* hud = new GameObject();
    HealthHUD* hpHud = new HealthHUD(*hud);
    hud->AddComponent(hpHud);
    hud->box.z = 3;
    AddObject(hud);
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
                    if (Collision::IsIsoColliding(
                            colliderA->box,
                            colliderB->box,
                            go->angleDeg,
                            go2->angleDeg
                            )) {
                        std::cout << "Real coll" << std::endl;
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
#ifdef TESTE
    Vec2 origin = IsoRect(Character::player->associated.box).Center();

    // Initialize points and lines vectors only with the camera rect
    std::vector<Vec2> points{Camera::pos, Camera::pos.Add({1200, 0}),
                                 Camera::pos.Add({1200, 900}), Camera::pos.Add({0, 900})};

    std::vector<Line> lines{Line(points[0], points[1]), Line(points[1], points[2]),
                                Line(points[2], points[3]), Line(points[3], points[0])};

    // Get all valid points/lines
    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        IsoCollider* collider = (IsoCollider*) go->GetComponent("IsoCollider");
        if (collider != nullptr && collider->opaque) {
            Vec2 objCenter = collider->box.Center();
            if (objCenter.x > Camera::pos.x && objCenter.x < Camera::pos.x + 1200 &&
                objCenter.y > Camera::pos.y && objCenter.y < Camera::pos.y + 900) {
                std::vector<Vec2> colPoints = collider->GetExtendedPoints();
                std::vector<Line> colLines = collider->GetLines();
                points.insert(points.end(), colPoints.begin(), colPoints.end());
                lines.insert(lines.end(), colLines.begin(), colLines.end());
            }
        }
    }

#ifdef DEBUG_VISIBILITY
    for (int i = 0; i < lines.size(); i++) {
        Line line = lines[i];
        SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawLine(GAME_RENDERER,
                            line.src.x - Camera::pos.x, line.src.y - Camera::pos.y,
                            line.dst.x - Camera::pos.x, line.dst.y - Camera::pos.y);
    }
#endif

    auto gen_vertex = [](float x, float y) {
        return SDL_Vertex {SDL_FPoint {x, y}, SDL_Color{ 255, 255, 255, 0 }, SDL_FPoint{ 0, 0 } };
    };

    // Create list with all intersection vertices
    std::vector<SDL_Vertex> vertices;
    for (int i = 0; i < points.size(); i++) {
        Ray::Intersection inter = Ray::ClosestIntersection(Line(origin, points[i]), lines);
        if (inter.exists) {
#ifdef DEBUG_VISIBILITY
            SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawLine(GAME_RENDERER,
                                origin.x - Camera::pos.x, origin.y - Camera::pos.y,
                                inter.pos.x - Camera::pos.x, inter.pos.y - Camera::pos.y);
#endif
            vertices.emplace_back(gen_vertex(inter.pos.x - Camera::pos.x, inter.pos.y - Camera::pos.y));
        }
    }

    origin = origin.Sub(Camera::pos);
    // Sort all vertices based on ray angle
    std::sort(vertices.begin(), vertices.end(), [origin](SDL_Vertex a, SDL_Vertex b) {
        return Vec2(a.position.x, a.position.y).Angle(origin) < Vec2(b.position.x, b.position.y).Angle(origin);
    });
    // First vertix is always origin
    vertices.insert(vertices.begin(), gen_vertex(origin.x, origin.y));

    /* Create indice vector
     * For every vertice we must have:
     * origin, vertice index, next vertice index
     */
    std::vector<int> indices;
    for (int i = 1; i < vertices.size(); i++) {
        indices.insert(indices.end(), { 0, i, i == vertices.size() - 1 ? 1 : i + 1 });
    }

    // Visibility layer
    SDL_Texture* visibility = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1200, 900);
    SDL_SetRenderTarget(GAME_RENDERER, visibility);

    // Black everything
    SDL_Rect screenRect;
    screenRect.x = screenRect.y = 0;
    screenRect.w = 1200;
    screenRect.h = 900;
    SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 255);
    SDL_RenderFillRect(GAME_RENDERER, &screenRect);

    // Render white triangles
    SDL_SetTextureBlendMode(visibility, SDL_BLENDMODE_BLEND);
    SDL_RenderGeometry(GAME_RENDERER, nullptr, vertices.data(), vertices.size(), indices.data(), indices.size());

    // Light layer
    SDL_Texture* light = SDL_CreateTexture(GAME_RENDERER, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 1200, 900);
    SDL_SetRenderTarget(GAME_RENDERER, light);
    SDL_SetTextureBlendMode(light, SDL_BLENDMODE_BLEND);

    // Backlight gradient
    SDL_Texture* backlight = Resources::GetImage("Recursos/img/backlight.png");
    //SDL_RenderCopy(GAME_RENDERER, backlight, &screenRect, &screenRect);

    // Flashlight
    Vec2 mousePos = Vec2(InputManager::GetInstance().GetMouseX(), InputManager::GetInstance().GetMouseY());
    float mouseAngle = mousePos.Angle(origin) + M_PI * 0.5;
    int flashlightSize = 600;
    float flashlightAngle = M_PI / 8;
    Vec2 flLeft = Vec2(origin.x + flashlightSize * sin(mouseAngle - flashlightAngle), origin.y - flashlightSize * cos(mouseAngle - flashlightAngle));
    Vec2 flRight = Vec2(origin.x + flashlightSize * sin(mouseAngle + flashlightAngle), origin.y - flashlightSize * cos(mouseAngle + flashlightAngle));

    /*SDL_SetRenderDrawColor(GAME_RENDERER, 255, 0, 0, 255);
    SDL_RenderDrawLine(GAME_RENDERER, origin.x, origin.y, flashlightCenter.x, flashlightCenter.y);
    SDL_RenderDrawLine(GAME_RENDERER, origin.x, origin.y, flLeft.x, flLeft.y);
    SDL_RenderDrawLine(GAME_RENDERER, origin.x, origin.y, flRight.x, flRight.y);*/

    //std::vector<SDL_Vertex> lightVertices = { gen_vertex(origin.x, origin.y), gen_vertex(flLeft.x, flLeft.y), gen_vertex(flRight.x, flRight.y) };
    //return SDL_Vertex {SDL_FPoint {x, y}, SDL_Color{ 255, 255, 255, 0 }, SDL_FPoint{ 0, 0 } };
    std::vector<SDL_Vertex> lightVertices = {
        SDL_Vertex {SDL_FPoint {origin.x, origin.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 0.5, 0.5 } },
        SDL_Vertex {SDL_FPoint {flLeft.x, flLeft.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 1, 0.3 } },
        SDL_Vertex {SDL_FPoint {flRight.x, flRight.y}, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 1, 0.7 } }
    };
    SDL_BlendMode bm;
    SDL_GetRenderDrawBlendMode(GAME_RENDERER, &bm);
    std::cout << bm << std::endl;
    SDL_Texture* flashlight = Resources::GetImage("Recursos/img/flashlight.png");
    int r = SDL_RenderGeometry(GAME_RENDERER, flashlight, lightVertices.data(), lightVertices.size(), nullptr, 0);

    std::cout << r << " " << SDL_GetError() << std::endl;
    //SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDOPERATION_MAXIMUM, );

    //SDL_Texture* flashlight = Resources::GetImage("Recursos/img/flashlight.png");
    SDL_Rect flSrcRect;
    flSrcRect.x = 0;
    flSrcRect.y = 0;
    SDL_Rect flDstRect;
    flDstRect.x = 450;
    flDstRect.y = 242;
    flDstRect.w = flSrcRect.w = 450;
    flDstRect.h = flSrcRect.h = 415;
    //SDL_RenderCopy(GAME_RENDERER, flashlight, &flSrcRect, &flDstRect);
    //SDL_RenderCopy(GAME_RENDERER, backlight, &screenRect, &screenRect);


    // Render visibility layer onto light layer
    //SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(GAME_RENDERER, visibility, &screenRect, &screenRect);
    SDL_SetRenderTarget(GAME_RENDERER, nullptr);
    SDL_RenderCopy(GAME_RENDERER, light, &screenRect, &screenRect);
    //SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_NONE);
#endif
}

void StageState::Pause() {}
void StageState::Resume() {}
