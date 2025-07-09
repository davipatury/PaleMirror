#include "hud/DialogueHUD.h"
#include "core/Game.h"

#define DIALOGUE_WRAP_LEN 910
#define TEXT_COOLDOWN 0.005
#define PICTURE_SPEED 500

std::string DialogueHUD::currentDialogue;
int DialogueHUD::currentLine;
bool DialogueHUD::startLine;

DialogueHUD::DialogueHUD(GameObject& associated) : Component(associated),
    caixaDialogo("Recursos/img/hud/caixa_dialogo.png"),
    helenaAssustada("Recursos/img/hud/helena-assustada.png"),
    helenaBrava("Recursos/img/hud/helena-brava.png"),
    helenaFeliz("Recursos/img/hud/helena-feliz.png"),
    helenaNeutra("Recursos/img/hud/helena-neutra.png")
{
    caixaDialogo.SetCameraFollower(true);
    helenaAssustada.SetCameraFollower(true);
    helenaBrava.SetCameraFollower(true);
    helenaFeliz.SetCameraFollower(true);
    helenaNeutra.SetCameraFollower(true);

    dialogueText = new TextHUD({238, 555}, "Recursos/font/PixelifySans-SemiBold.ttf", 40, TextHUD::BLENDED, "A", {0, 0, 0, 255}, DIALOGUE_WRAP_LEN);
    charNameText = new TextHUD({255, 480}, "Recursos/font/PixelifySans-SemiBold.ttf", 50, TextHUD::BLENDED, "B", {255, 255, 255, 255});

    // Cada dialogo é composto por uma chave e por uma array de falas
    // Nesse caso, o dialogo "test" tem 3 falas
    // Cada fala tem um sprite (opcional, ex.: uma fala de um radio nao precisa de sprite), nome de quem ta falando, cor de quem ta falando, o texto a ser falado, e resetPos (opcional)
    // A primeira fala usa o sprite "HelenaAssustada", o nome do falante é "Helena", tem uma cor do falante cor especifico, e o texto lorem ipsum.
    // O ultimo parametro que é o resetPos, pode ser utilizado em um dialogo com mais de uma pessoa por exemplo, onde a animacao do sprite entrando na tela é refeita em uma fala especifica.
    dialogueLines["test"] = {
        {"HelenaAssustada", "Helena", {101, 38, 141, 255},
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur tincidunt ultrices mi non suscipit. Suspendisse vitae consequat turpis. Donec bibendum velit felis, in sodales ipsum eleifend sit amet."
        },
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
        "Aliquam efficitur nulla ipsum, eget consectetur diam lacinia ac. Sed id nunc nisl. Pellentesque interdum massa nec risus imperdiet, in pulvinar odio rutrum."
        },
        {"", "Radio", {0, 0, 0, 255},
        "*estatica* *estatica* *estatica*"
        }
    };
    // Para adicionar um novo dialogo é só repetir a estrutura acima mudando o que for necessário e para chamar o dialogo é só usar DialogueHUD::RequestDialogue("nova_chave");

    textTimer = Timer();
    currentTextPos = 1;

    currentDialogue = "";
    currentLine = 0;
    startLine = false;

    picturePos.y = 214;
}

DialogueHUD::~DialogueHUD() {}

void DialogueHUD::Update(float dt) {
    if (!currentDialogue.empty()) {
        CURRENT_STATE.openUI = true;
        textTimer.Update(dt);
        bool skipFirstKeyCheck = startLine;

        DialogueLine dialLine = dialogueLines.at(currentDialogue)[currentLine];
        if (startLine) {
            if (dialLine.picture == "HelenaAssustada") pictureSprite = &helenaAssustada;
            else if (dialLine.picture == "HelenaBrava") pictureSprite = &helenaBrava;
            else if (dialLine.picture == "HelenaFeliz") pictureSprite = &helenaFeliz;
            else if (dialLine.picture == "HelenaNeutra") pictureSprite = &helenaNeutra;
            else pictureSprite = nullptr;

            if (pictureSprite && ((startLine && currentLine == 0) || dialLine.resetPos)) picturePos.x = -pictureSprite->GetWidth();
            else picturePos.x = 0;

            currentTextPos = 0;
            dialogueText->SetHiding(true);
            charNameText->SetColor(dialLine.charNameColor);
            charNameText->SetText(dialLine.charName);
            startLine = false;
        }

        if (picturePos.x != 0) {
            picturePos.x = std::min(picturePos.x + PICTURE_SPEED * dt, 0.0f);
        } else {
            dialogueText->SetHiding(false);
            if (currentTextPos == 0 || (textTimer.Get() > TEXT_COOLDOWN && currentTextPos <= dialLine.line.length())) {
                dialogueText->SetText(dialLine.line.substr(0, currentTextPos));
                textTimer.Restart();
                currentTextPos++;
            }
        }

        if (INPUT_MANAGER.AnyKeyPress() && !skipFirstKeyCheck) {
            if (picturePos.x != 0) picturePos.x = 0;
            else if (currentTextPos < dialLine.line.length()) currentTextPos = dialLine.line.length();
            else {
                currentLine++;
                if (currentLine >= dialogueLines.at(currentDialogue).size()) {
                    currentDialogue = "";
                    CURRENT_STATE.openUI = false;
                } else startLine = true;
            }
        }
    }
}

void DialogueHUD::Render() {
    if (!currentDialogue.empty()) {
        if (pictureSprite != nullptr) {
            pictureSprite->Render(picturePos.x, picturePos.y, pictureSprite->GetWidth(), pictureSprite->GetHeight());
        }
        caixaDialogo.Render(201, 458, caixaDialogo.GetWidth(), caixaDialogo.GetHeight());
        dialogueText->Render();
        charNameText->Render();
    }
}

bool DialogueHUD::Is(std::string type) {
    return type == "DialogueHUD";
}

void DialogueHUD::RequestDialogue(std::string dialogueKey) {
    currentDialogue = dialogueKey;
    currentLine = 0;
    startLine = true;
}
