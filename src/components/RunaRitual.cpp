#include "components/RunaRitual.h"

#include "core/Game.h"

#

RunaRitual::RunaRitual(GameObject &associated, bool sempreCompleto) : Component(associated), velas("Recursos/img/objetos/velas.png", 4, 1) {
    // Set color
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    SDL_SetTextureBlendMode(sr->sprite.texture, SDL_BLENDMODE_BLEND);

    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    intr->SetHUDOffset({65, 10});
    this->sempreCompleto = sempreCompleto;
}

// States actions
void RunaAlagada(State* state, GameObject* associated) {
    DialogueHUD::RequestDialogue("waterOn");
    //std::cout << "Preciso remover essa agua" << std::endl;
}

void RunaVazia(State* state, GameObject* associated) {
    if (INVENTORY->HasItem(ITEM_BALDE_TINTA)) {
        DialogueHUD::RequestDialogue("runaComBalde");
    } else {
        DialogueHUD::RequestDialogue("runaSemBalde");
    }
}

void PintarRuna(State* state, GameObject* associated) {
    INVENTORY->Remove(ITEM_BALDE_TINTA);
    GameData::runeState = GameData::RUNA_DESENHADA;
    DialogueHUD::RequestDialogue("runaDesenhada");
}

void RunaSemVela(State* state, GameObject* associated) {
    if (INVENTORY->HasItem(ITEM_VELA)) {
        DialogueHUD::RequestDialogue("runaComVela");
    } else {
        DialogueHUD::RequestDialogue("runaSemVela");
    }
}

void ColocarVela(State* state, GameObject* associated) {
    INVENTORY->Remove(ITEM_VELA);
    if (GameData::runeState == GameData::RUNA_DESENHADA) {
        GameData::runeState = GameData::RUNA_COM_UMA_VELA;
    } else {
        GameData::runeState = GameData::RUNA_FINALIZADA;
    }
}

void LigarRuna(State* state, GameObject* associated) {
    DialogueHUD::RequestDialogue("runaLigada");
    GameData::runeState = GameData::RUNA_LIGADA;
}

void RunaRitual::Update(float dt) {
    SpriteRenderer* sr = (SpriteRenderer*) associated.GetComponent("SpriteRenderer");
    Interactable* intr = (Interactable*) associated.GetComponent("Interactable");
    LightEmitter* light = (LightEmitter*) associated.GetComponent("LightEmitter");
    if (sempreCompleto && sr && light) {
        sr->SetFrame(1);
        SDL_Color corRitual = PaintPuzzle::GetSolutionColor();
        sr->SetColorMod(corRitual.r, corRitual.g, corRitual.b);
        velas.SetFrame(3);
        light->SetEnabledAll(true);
        return;
    }
    if (!sr || !intr || !light) return;

    switch (GameData::runeState) {
    case GameData::RUNA_ALAGADA: {
        sr->SetFrame(2);
        velas.SetFrame(0);
        intr->SetHUDText("Inspecionar");
        intr->SetAction(RunaAlagada);
        break;
    }
    case GameData::RUNA_VAZIA: {
        sr->SetFrame(0);
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
        sr->SetFrame(1);
        SDL_Color corRitual = PaintPuzzle::GetSolutionColor();
        sr->SetColorMod(corRitual.r, corRitual.g, corRitual.b);
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
        sr->SetFrame(1);
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
        sr->SetFrame(1);
        velas.SetFrame(2);
        intr->SetType(InteractableHUD::INTERACT);
        intr->SetHUDText("Finalizar");
        intr->SetAction(LigarRuna);
        break;
    }
    case GameData::RUNA_LIGADA:
    case GameData::RUNA_SUMMONADA: {
        sr->SetFrame(1);
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
