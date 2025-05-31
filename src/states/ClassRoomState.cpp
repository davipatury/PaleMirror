#include "states/ClassRoomState.h"
#include "states/StageState.h"
#include "actions/NewStateAction.h"
#include "actions/Action.h"
#include "actions/BackStateAction.h"
#include "components/Interactable.h"
#include "physics/IsoCollider.h"

ClassRoomState::ClassRoomState() {
    started = false;
    quitRequested = false;
    popRequested = false;
}

ClassRoomState::~ClassRoomState(){
    Camera::Unfollow();
    objectArray.clear();
}

void ClassRoomState::LoadAssets() {
    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer((*bg), "Recursos/img/rooms/ClassRoom.png", 1, 1);
    //bgSprite->SetCameraFollower(true);
    bgSprite->SetScale(0.50, 0.50);
    bg->AddComponent(bgSprite);
    bg->box.x = 0;
    bg->box.y = 100;
    bg->box.z = -2;
    AddObject(bg);

    // Mesa e Cadeira
    GameObject* mesacad = new GameObject();
    SpriteRenderer* mcSprite = new SpriteRenderer(*mesacad, "Recursos/img/obj/CADEIRAeMESA.png");
    mcSprite->SetScale(1.25, 1.25);
    mesacad->AddComponent(mcSprite);
    mesacad->AddComponent(new IsoCollider(*mesacad, {1, 1}, {0.2, 0.2}));
    
    mesacad->box.x = 740;
    mesacad->box.y = 640;
    mesacad->box.z = 0;
    AddObject(mesacad);

    // Porta
    GameObject* backButton = new GameObject();
    backButton->box.x = 500;
    backButton->box.y = 600;
    backButton->box.z = 0;
    auto backSprite = new SpriteRenderer(*backButton, "Recursos/img/obj/espelho.png");
    backSprite->SetScale(1.8, 1.8);
    backButton->AddComponent(backSprite);
    backButton->AddComponent(new IsoCollider(*backButton, {0.5, 0.5}, {0, 0}));
    std::unique_ptr<Action> backAction(new BackStateAction(&popRequested));
    Interactable* backInteract = new Interactable(*backButton, std::move(backAction));
    backInteract->SetRequireMouseOver(true);
    backButton->AddComponent(backInteract);
    AddObject(backButton);

    // Player
    GameObject* character = new GameObject();
    Character* charCmp = new Character((*character), "Recursos/img/Player.png");
    character->AddComponent(charCmp);
    PlayerController* playerController = new PlayerController(*character);
    character->AddComponent(playerController);
    character->box.x = 640;
    character->box.y = 835;
    character->box.z = 0;
    Character::player = charCmp;
    //Camera::Follow(character);
    Camera::Follow(bg);
    AddObject(character);
}

void ClassRoomState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void ClassRoomState::Update(float dt) {
    // Quit requested
    if (InputManager::GetInstance().IsKeyDown(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested()) {
        quitRequested = true;
    }

    // Start
    if (InputManager::GetInstance().IsKeyDown(SDLK_SPACE)) {
        Game::GetInstance().Push(new StageState());
    }

    // Update camera
    Camera::Update(dt);
    std::cout<<Character::player->Pos().x<<" "<<Character::player->Pos().y<<std::endl;
    UpdateArray(dt);
}

void ClassRoomState::Render() {
    RenderArray();
}

void ClassRoomState::Pause() {
}

void ClassRoomState::Resume() {
}
