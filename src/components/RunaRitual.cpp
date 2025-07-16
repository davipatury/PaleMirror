#include "components/RunaRitual.h"

#include "core/Game.h"

#

RunaRitual::RunaRitual(GameObject &associated) : Component(associated), velas("Recursos/img/objetos/velas.png", 4, 1) {
    // Set color
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    SDL_Color corRitual = PaintPuzzle::GetSolutionColor();
    sr->SetColorMod(corRitual.r, corRitual.g, corRitual.b);

    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    intr->SetHUDOffset({65, 10});
}

// States actions
void RunaAlagada(State* state, GameObject* associated) {
    std::cout << "Preciso remover essa agua" << std::endl;
}

void RunaVazia(State* state, GameObject* associated) {
    if (INVENTORY->HasItem(ITEM_BALDE_TINTA)) {
        std::cout << "Preciso usar balde de tinta" << std::endl;
    } else {
        std::cout << "Preciso encontrar balde de tinta" << std::endl;
    }
}

void PintarRuna(State* state, GameObject* associated) {
    INVENTORY->Remove(ITEM_BALDE_TINTA);
    GameData::runeState = GameData::RUNA_DESENHADA;
    std::cout << "Runa pintada" << std::endl;
}

void RunaSemVela(State* state, GameObject* associated) {
    if (INVENTORY->HasItem(ITEM_VELA)) {
        std::cout << "Preciso usar vela" << std::endl;
    } else {
        std::cout << "Preciso encontrar vela" << std::endl;
    }
}

void ColocarVela(State* state, GameObject* associated) {
    INVENTORY->Remove(ITEM_VELA);
    if (GameData::runeState == GameData::RUNA_DESENHADA) {
        std::cout << "Falta 1 vela" << std::endl;
        GameData::runeState = GameData::RUNA_COM_UMA_VELA;
    } else {
        std::cout << "Tudo ok pro ritual" << std::endl;
        GameData::runeState = GameData::RUNA_FINALIZADA;
    }
}

void LigarRuna(State* state, GameObject* associated) {
    std::cout << "Runa ligada" << std::endl;
    GameData::runeState = GameData::RUNA_LIGADA;
}

void RunaRitual::Update(float dt) {
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    LightEmitter* light = (LightEmitter*) associated.GetComponent("LightEmitter");
    if (!sr || !intr || !light) return;

    switch (GameData::runeState) {
    case GameData::RUNA_ALAGADA: {
        sr->SetVisible(false);
        velas.SetFrame(0);
        intr->SetHUDText("Inspecionar");
        intr->SetAction(RunaAlagada);
        break;
    }
    case GameData::RUNA_VAZIA: {
        sr->SetVisible(false);
        velas.SetFrame(0);
        if (INVENTORY->HasItemInHand(ITEM_BALDE_TINTA)) {
            intr->SetType(InteractableHUD::USE_ITEM);
            intr->SetHUDText("Pintar");
            intr->SetAction(PintarRuna);
        } else {
            intr->SetType(InteractableHUD::INTERACT);
            intr->SetHUDText("Inspecionar");
            intr->SetAction(RunaVazia);
        }
        break;
    }
    case GameData::RUNA_DESENHADA: {
        sr->SetVisible(true);
        velas.SetFrame(0);
        if (INVENTORY->HasItemInHand(ITEM_VELA)) {
            intr->SetType(InteractableHUD::USE_ITEM);
            intr->SetHUDText("Colocar");
            intr->SetAction(ColocarVela);
        } else {
            intr->SetType(InteractableHUD::INTERACT);
            intr->SetHUDText("Inspecionar");
            intr->SetAction(RunaSemVela);
        }
        break;
    }
    case GameData::RUNA_COM_UMA_VELA: {
        sr->SetVisible(true);
        velas.SetFrame(1);
        if (INVENTORY->HasItemInHand(ITEM_VELA)) {
            intr->SetType(InteractableHUD::USE_ITEM);
            intr->SetHUDText("Colocar");
            intr->SetAction(ColocarVela);
        } else {
            intr->SetType(InteractableHUD::INTERACT);
            intr->SetHUDText("Inspecionar");
            intr->SetAction(RunaSemVela);
        }
        break;
    }
    case GameData::RUNA_FINALIZADA: {
        sr->SetVisible(true);
        velas.SetFrame(2);
        intr->SetType(InteractableHUD::INTERACT);
        intr->SetHUDText("Finalizar");
        intr->SetAction(LigarRuna);
        break;
    }
    case GameData::RUNA_LIGADA: {
        sr->SetVisible(true);
        velas.SetFrame(3);
        intr->SetActivationDistance(0.0f);
        light->SetEnabledAll(true);
        break;
    }
    }
}

void RunaRitual::Start() { }

void RunaRitual::Render() {
    velas.Render(associated.box.x - 16, associated.box.y - 58, velas.GetWidth(), velas.GetHeight());
}

bool RunaRitual::Is(std::string type) {
    return type == "RunaRitual";
}
