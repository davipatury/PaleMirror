#include "states/StageState.h"
#include "components/Interactable.h"
#include "actions/Actions.h"
#include "hud/FlashlightHUD.h"
#include <memory>
#include "core/GameData.h"
#include "math.h"

StageState::StageState() {
    started = false;
    quitRequested = false;
    popRequested = false;
    zombieFarAwayTimer = Timer();
    zombieFarAwaySound = new Sound("Recursos/audio/sounds/monster/monstro2000-2.wav");
}

StageState::~StageState() {
    objectArray.clear();
}

void StageState::LoadAssets() {
    // Generate random paint puzzle solution
    PaintPuzzle::GenerateRandomSolution();

    /*
     * Game Objects
     */
    // Player
    GameObject* character = new GameObject("[PLAYER]");
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    character->AddComponent(new PlayerController(*character));
    character->AddComponent(charCmp);
    character->box.x = 2002;
    character->box.y = 1988;
    character->box.z = OBJECT_LAYER;
    Character::player = charCmp;
    Camera::Follow(character);
    AddObject(character);

    // Shadows

    GameObject* scc = new GameObject("[ShadowCasterController");
    scc->AddComponent(new ShadowCaster::Controller(*scc));
    scc->box.z = SHADOW_LAYER;
    scc->lazyRender = false;
    scc->pauseOnOpenUI = false;
    AddObject(scc);
    // HUD
    // Flashlight
    GameObject* flHUD = new GameObject("[FlashlightHUD]");
    FlashlightHUD* flComp = new FlashlightHUD(*flHUD);
    FlashlightHUD::instance = flComp;
    flHUD->AddComponent(flComp);
    flHUD->box.z = FLASHLIGHT_LAYER;
    flHUD->lazyRender = false;
    flHUD->pauseOnOpenUI = false;
    AddObject(flHUD);

    // General
    GameObject* hud = new GameObject("[HUD]");
    // HealthHUD
    hud->AddComponent(new HealthHUD(*hud));
    // InventoryHUD
    InventoryHUD* inv = new InventoryHUD(*hud);
    InventoryHUD::instance = inv;
    hud->AddComponent(inv);
    // InteractableHUD
    InteractableHUD* intr = new InteractableHUD(*hud);
    InteractableHUD::instance = intr;
    hud->AddComponent(intr);
    // DebugHUD
    hud->AddComponent(new DebugHUD(*hud));
    hud->box.z = HUD_LAYER;
    hud->lazyRender = false;
    hud->pauseOnOpenUI = false;
    AddObject(hud);
    // Pause
    GameObject* pauseHUD = new GameObject("[PauseHUD]");
    pauseHUD->AddComponent(new PauseHUD(*pauseHUD));
    pauseHUD->box.z = HUD_LAYER;
    pauseHUD->lazyRender = false;
    AddObject(pauseHUD);
    // Dialogue
    GameObject* dHud = new GameObject("[DialogueHUD]");
    dHud->AddComponent(new DialogueHUD(*dHud));
    dHud->box.z = DIALOGUE_LAYER;
    dHud->lazyRender = false;
    dHud->pauseOnOpenUI = false;
    AddObject(dHud);

    /*
     * Rooms
     */
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
    BanheiroFemininoRoom* banheiroFemRoom = new BanheiroFemininoRoom(this);
    banheiroFemRoom->Build();
    BanheiroMasculinoRoom* banheiroMascRoom = new BanheiroMasculinoRoom(this);
    banheiroMascRoom->Build();
    BanheiroIntroRoom* banheiroIntroRoom = new BanheiroIntroRoom(this);
    banheiroIntroRoom->Build();
    BanheiroDemRoom* banheiroDemRoom = new BanheiroDemRoom(this);
    banheiroDemRoom->Build();
    DiretorRoom* diretorRoom = new DiretorRoom(this);
    diretorRoom->Build();
    ProfessoresRoom* professoresRoom = new ProfessoresRoom(this);
    professoresRoom->Build();

    rooms["main"] = mainRoom;
    rooms["history"] = historyRoom;
    rooms["arts"] = artsRoom;
    rooms["science"] = scienceRoom;
    rooms["portuguese"] = portugueseRoom;
    rooms["banheiroFem"] = banheiroFemRoom;
    rooms["banheiroMasc"] = banheiroMascRoom;
    rooms["banheiroIntro"] = banheiroIntroRoom;
    rooms["banheiroDem"] = banheiroDemRoom;
    rooms["diretor"] = diretorRoom;
    rooms["professores"] = professoresRoom;

    currentRoom = mainRoom;
    //mainRoom->bgMusic.FadeInPos(0.0);
    mainRoom->Enter();

    // Give flashlight on begin (REMOVE LATER)
    INVENTORY->Collect(ITEM_LANTERNA);
    INVENTORY->Collect(ITEM_CANO);
}

void StageState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void StageState::Update(float dt) {
    // Quit requested
    if (INPUT_MANAGER.QuitRequested()) {
        quitRequested = true;
    }

    // Debug delete monsters
    if (INPUT_MANAGER.KeyPress(SDLK_DELETE)) {
        for (int i = 0; i < objectArray.size(); i++) {
            GameObject* go = objectArray[i].get();
            if (go->GetComponent("Zombie") != nullptr || go->GetComponent("AIController") != nullptr) {
                go->RequestDelete();
            }
        }
    }

    // Fullscreen
    if (INPUT_MANAGER.KeyPress(SDLK_F11)) {
        Uint32 flags = SDL_GetWindowFlags(GAME_WINDOW);
        bool isFullscreen = flags & SDL_WINDOW_FULLSCREEN;
        SDL_SetWindowFullscreen(GAME_WINDOW, isFullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
    }

    if (INPUT_MANAGER.KeyPress('y')) {
        std::cout << "Finge que arrumei os canos la" << std::endl;
        GameData::runeState = GameData::RUNA_VAZIA;
    }

    // Dialogue debug
    if (!openUI && INPUT_MANAGER.KeyPress('o')) {
        DialogueHUD::RequestDialogue("test");
    }

    // Spawn mirror puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('m')) {
        GameObject* mp = new GameObject();
        mp->AddComponent(new MirrorPuzzle((*mp), std::vector<MirrorPuzzle::Piece>{
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/1.png", Vec2{0, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/2.png", Vec2{81, 0}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/3.png", Vec2{0, 198}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/4.png", Vec2{57, 148}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/5.png", Vec2{0, 290}),
            MirrorPuzzle::Piece("Recursos/img/mirror_puzzle/6.png", Vec2{184, 382})
        }));
        mp->box.z = PUZZLE_LAYER;
        mp->lazyRender = false;
        mp->pauseOnOpenUI = false;
        AddObject(mp);
    }

    // Spawn mirrorzinho puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('m')) {
        GameObject* mp = new GameObject();
        mp->AddComponent(new MirrorzinhoPuzzle((*mp), std::vector<MirrorzinhoPuzzle::Piece>{
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/1.png", Vec2{0, 0}),
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/2.png", Vec2{81, 0}),
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/3.png", Vec2{0, 198}),
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/4.png", Vec2{57, 148}),
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/5.png", Vec2{0, 290}),
            MirrorzinhoPuzzle::Piece("Recursos/img/mirror_puzzle/6.png", Vec2{184, 382})
        }));
        mp->box.z = PUZZLE_LAYER;
        mp->lazyRender = false;
        mp->pauseOnOpenUI = false;
        AddObject(mp);
    }

    // Spawn fuse puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('f')) {
        GameObject* fp = new GameObject();
        fp->AddComponent(new FusePuzzle(*fp));
        fp->box.z = PUZZLE_LAYER;
        fp->lazyRender = false;
        fp->pauseOnOpenUI = false;
        AddObject(fp);
    }

    // Spawn paint puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('p')) {
        GameObject* pp = new GameObject();
        pp->AddComponent(new PaintPuzzle(*pp));
        pp->box.z = PUZZLE_LAYER;
        pp->lazyRender = false;
        pp->pauseOnOpenUI = false;
        AddObject(pp);
    }

    // Spawn pipe puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('i')) {
        GameObject* pip = new GameObject();
        pip->AddComponent(new PipePuzzle(*pip));
        pip->box.z = PUZZLE_LAYER;
        pip->lazyRender = false;
        pip->pauseOnOpenUI = false;
        AddObject(pip);
    }

    //  Scenes
    if (!openUI && INPUT_MANAGER.KeyPress('b')) {
        Actions::ChangeRoom("banheiroIntro")(this, nullptr);
        FLASHLIGHT->SetDark(false);
        DialogueHUD::RequestDialogue("prologoPreRitual");
        scene = RITUAL_BEFORE;
        DialogueHUD::RequestDialogue("prologoRitual");
    }

    if (scene == RITUAL_BEFORE && DialogueHUD::isEmpty()){
        Actions::ChangeRoom("banheiroDem")(this, nullptr);
        scene = RITUAL_DURING;
        FLASHLIGHT->SetDark(true);
    }

    if(scene == RITUAL_DURING && DialogueHUD::isEmpty){
        DialogueHUD::RequestDialogue("prologoPosRitual");
        scene = RITUAL_AFTER;
    }

    if(introplaying and !Mix_PlayingMusic()){
        backgroundMusic.Open("Recursos/audio/music/boss-loop.wav");
        backgroundMusic.Play();
        Mix_VolumeMusic(70);
    }

    if(Boss::startBoss){
        Mix_VolumeMusic(80);
        backgroundMusic.Open("Recursos/audio/music/boss-intro.wav");
        backgroundMusic.Play(1);
        introplaying = true;
        DialogueHUD::RequestDialogue("boss_battle");
        Boss::startBoss = false;
    }

    // Custcene Boss
    if (!openUI && (INPUT_MANAGER.KeyPress('k') || (GameData::runeState == GameData::RUNA_LIGADA && GameData::mirrorPuzzleSolved))) {
        Character::player->associated.box.x = 2257;
        Character::player->associated.box.y = 2500;

        GameObject* aneleh = new GameObject("[Aneleh]");
        aneleh->box.x = 2100;
        aneleh->box.y = 2303;
        aneleh->AddComponent(new Boss(*aneleh));
        AddObject(aneleh);
        Boss::startBoss = true;
    }

    
    
    // Spawn locker puzzle
    if (!openUI && INPUT_MANAGER.KeyPress('l')) {
        GameObject* lp = new GameObject();
        lp->AddComponent(new LockPuzzle(*lp, "1234"));
        lp->box.z = PUZZLE_LAYER;
        lp->lazyRender = false;
        lp->pauseOnOpenUI = false;
        AddObject(lp);
    }

    if(GameData::zombieFarAway and zombieFarAwayTimer.Get() > 15) {
        GameData::zombieFarAway = false;
        zombieFarAwayTimer.Restart();
        zombieFarAwaySound->Play(0);
    }

    // Update game objects
    zombieFarAwayTimer.Update(dt);
    UpdateArray(dt);

    // Collision

    for (int i = 0; i < objectArray.size(); i++) {
        GameObject* go = objectArray[i].get();
        Collider* colliderA = (Collider*) go->GetComponent("Collider");
        if (colliderA != nullptr) {
            for (int j = i + 1; j < objectArray.size(); j++) {
                GameObject* go2 = objectArray[j].get();
                Collider* colliderB = (Collider*) go2->GetComponent("Collider");

                HitAttack* hitA = (HitAttack*) go->GetComponent("HitAttack");
                HitAttack* hitB = (HitAttack*) go2->GetComponent("HitAttack");
                if (colliderB != nullptr && (hitA != nullptr || hitB != nullptr)) {
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
}

void StageState::Pause() {
    pauseX = Character::player->Pos().x;
    pauseY = Character::player->Pos().y;

}
void StageState::Resume() {
    Camera::Follow(&Character::player->associated);
}

void StageState::ChangeRoom(std::string room) {
    if (currentRoom != nullptr) currentRoom->Leave();
    currentRoom = rooms[room];
    currentRoom->Enter();
}

Room* StageState::GetCurrentRoom() {
    return currentRoom;
}

Room* StageState::GetRoom(std::string room) {
    return rooms[room];
}
