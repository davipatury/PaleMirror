#include "components/HealthHUD.h"


HealthHUD::HealthHUD(GameObject& associated) : Component(associated), dmg1("Recursos/img/hud/dmg1.png"), dmg2("Recursos/img/hud/dmg2.png"), dmg3("Recursos/img/hud/dmg3.png"), 
    toolbar("Recursos/img/hud/toolbar.png"), toolbarselect("Recursos/img/hud/toolbar-selector.png") {
    selToolbar = 0;
    dmg1.SetCameraFollower(true);
    dmg2.SetCameraFollower(true);
    dmg3.SetCameraFollower(true);
    toolbar.SetCameraFollower(true);
    toolbarselect.SetCameraFollower(true);
}

HealthHUD::~HealthHUD() {}

void HealthHUD::Update(float dt) {

    /*
    if (INPUT_MANAGER.KeyPress(SDLK_1)) {
        selToolbar = 0;
    }else if( INPUT_MANAGER.KeyPress(SDLK_2)) {
        selToolbar = 1;
    }else if(INPUT_MANAGER.KeyPress(SDLK_3)) {
        selToolbar = 2;
    } else{
        selToolbar = 3;
    } 
    */
    if (INPUT_MANAGER.KeyPress(SDLK_q)) {
        selToolbar--;
        selToolbar += 4;
        selToolbar %= 4;
    }

    if (INPUT_MANAGER.KeyPress(SDLK_e)) {
        selToolbar++;
        selToolbar += 4;
        selToolbar %= 4;
    }
}

void HealthHUD::Render() {
    if (Character::player == nullptr) return;
    int hp = Character::player->GetHP();
    if (hp < 40) {
        dmg3.Render(0, 0, 1200, 900);
    } else if (hp < 65) {
        dmg2.Render(0, 0, 1200, 900);
    } else if (hp < 80) {
        dmg1.Render(0, 0, 1200, 900);
    }

    // toolbar = 523x156
    // toolbarselector = 108x117

    toolbar.SetScale(0.5,0.5);
    toolbar.Render(460, 760, toolbar.GetWidth(), toolbar.GetHeight());

    toolbarselect.SetScale(0.5, 0.5);
    toolbarselect.Render(467 + selToolbar * (9 + toolbarselect.GetWidth()), 769, toolbarselect.GetWidth(), toolbarselect.GetHeight());
}

bool HealthHUD::Is(std::string type) {
    return type == "HealthHUD";
}
