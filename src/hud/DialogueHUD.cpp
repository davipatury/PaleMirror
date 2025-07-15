#include "hud/DialogueHUD.h"
#include "core/Game.h"

#define DEBUG_DIALOGUE

#define DIALOGUE_WRAP_LEN 910
#define TEXT_COOLDOWN 0.005
#define PICTURE_SPEED 750

std::queue<DialogueHUD::DialogueQuery> DialogueHUD::dialogueQueue;

DialogueHUD::DialogueHUD(GameObject& associated) : Component(associated),
    caixaDialogo("Recursos/img/hud/caixa_dialogo.png", 1, 1, true),
    helenaAssustada("Recursos/img/hud/helena-assustada.png", 1, 1, true),
    helenaBrava("Recursos/img/hud/helena-brava.png", 1, 1, true),
    helenaFeliz("Recursos/img/hud/helena-feliz.png", 1, 1, true),
    helenaNeutra("Recursos/img/hud/helena-neutra.png", 1, 1, true),
    madu("Recursos/img/hud/madu.png", 1, 1, true),
    maju("Recursos/img/hud/maju.png", 1, 1, true),
    malu("Recursos/img/hud/malu.png", 1, 1, true)
{
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
        /*{"", "Radio", {0, 0, 0, 255},
        "*estatica* *estatica* *estatica*"
        },*/
        {"Madu", "Madu", {167, 8, 8, 255}, "madu", true},
        {"Maju", "Maju", {118, 191, 27, 255}, "maju"},
        {"Malu", "Malu", {223, 197, 62, 255}, "malu"}
    };

    // Paint puzzle lines
    dialogueLines["paintPuzzle_corErrada"] = {
        {"HelenaNeutra", "Helena", {101, 38, 141, 255},
        "errei familia fui mlk"
        }
    };
    dialogueLines["paintPuzzle_solved"] = {
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
        "acertei porra"
        }
    };

    // Mirror puzzle
    dialogueLines["mirrorPuzzle_solved"] = {
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
            "ta arrumado o espelho"
        }
    };

    // Fuse puzzle
    dialogueLines["fusePuzzle_solved"] = {
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
            "ta arrumado a caixa de fusivel"
        }
    };

    // Pipe puzzle
    dialogueLines["pipePuzzle_solved"] = {
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
            "ta arrumado os canos (que cano o.O)"
        }
    };

    dialogueLines["prologo_marias"] = {
        {"HelenaAssustada", "Helena", {101, 38, 141, 255},
        "E se alguem da escola nos ver aqui?", true
        },
        {"Maju", "Maju", {101, 38, 141, 255},
        "Cala a boca! É por isso que a gente esperou todo mundo sair para invadir a escola, cabeça de pirulito.", true
        },
        {"Malu", "Malu", {101, 38, 141, 255},
        "Está com medinho do ritual Helena? Achei que você queria entrar para o nosso grupinho...", true
        },
        {"Madu", "Madu", {101, 38, 141, 255},
        "Ela está com medinho hihihihi", true
        },
        {"Helena", "Helena", {101, 38, 141, 255},
        "Não estou!", true
        },
        {"Malu", "Malu", {101, 38, 141, 255},
        "Então prove para gente, vai na frente e faça o ritual.", true
        },
        {"HelenaNeutra", "Helena", {101, 38, 141, 255},
        "Aliquam efficitur nulla ipsum, eget consectetur diam lacinia ac. Sed id nunc nisl. Pellentesque interdum massa nec risus imperdiet, in pulvinar odio rutrum."
        },

        //barulho de tranca

        {"Madu", "Madu", {101, 38, 141, 255},
        "hihihihi", true
        },
        {"HelenaAssustada", "Helena", {101, 38, 141, 255},
        "O QUE VOCÊS ESTÃO FAZENDO? ABREM A PORTA! POR FAVOR!", true
        },
        {"Maju", "Maju", {101, 38, 141, 255},
        "Você sempre têm ideias tão boas Malu!", true
        },
        {"Malu", "Malu", {101, 38, 141, 255},
        "Eu sei, eu sou perfeita", true
        },
        {"Madu", "Madu", {101, 38, 141, 255},
        "bye bye", true
        },
        
        //tela pisca e barulhos sinistros
    };

    dialogueLines["prologo_vila"] = {
        {"Vila", "Vila", {101, 38, 141, 255},
        "Helena? Você está bem?", true
        },
        {"HelenaAssustada", "Helena", {101, 38, 141, 255},
        "AAAAAAAAAAAAAAAAAAAAA", true
        },
        {"HelenaAssustada", "Helena", {101, 38, 141, 255},
        "Q-quem é vo-você? C-como sabe meu nome?",
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Desculpe te assustar, me chamo Eneleh, você acabou desmaiando e eu fiquei preocupada... eu vi o que elas fizeram com você.", true
        },
        {"HelenaBrava", "Helena", {101, 38, 141, 255},
        "Ughh, eu fui TÃO ingênua, mas obrigada pela preocupação", true
        },
        {"HelenaNeutra", "Helena", {101, 38, 141, 255},
        "Mas e como você foi parar aí dentro do espelho?",
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Ahhh isso é uma longa história, me prenderam nesse espelho há muitos anos atrás", true
        },
        {"Helena", "Helena", {101, 38, 141, 255},
        "Assim, estamos trancadas aqui até amanhã, então temos tempo", true
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Verdadade. Desculpe, é que não estou acostumada com isso. Eu nunca tive uma amiga, sempre fui excluída e zombavam de mim... você entende né?", true
        },
        {"Helana", "Helena", {101, 38, 141, 255},
        "Err, te entendo bem", true
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Fico muito feliz de ter te conhecido Helena.", true
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Antes de te contar a história, tem como me tirar daqui?",
        },
        {"Helena", "Helena", {101, 38, 141, 255},
        "...", true
        },
        {"Vila", "Vila", {101, 38, 141, 255},
        "Esse espelho está um pouco quebrado, preciso de sua ajuda para conserta-lo, amiga", true
        },
        {"HelenaFeliz", "Helena", {101, 38, 141, 255},
        "A-amiga?", true
        },
        {"HelenaNeutra", "Helena", {101, 38, 141, 255},
        "Pode deixar comigo!",
        }
    };

    // Para adicionar um novo dialogo é só repetir a estrutura acima mudando o que for necessário e para chamar o dialogo é só usar DialogueHUD::RequestDialogue("nova_chave");

    textTimer = Timer();
    currentTextPos = 1;

    currentDialogue = "";
    dialogueEndFunc = nullptr;
    currentLine = 0;
    startLine = false;

    picturePos.y = 214;
}

DialogueHUD::~DialogueHUD() {}

void DialogueHUD::Update(float dt) {
    if (!dialogueQueue.empty() && currentDialogue.empty()) {
        DialogueQuery dq = dialogueQueue.front();
#ifdef DEBUG_DIALOGUE
        std::cout << "[DialogueHUD] Dequeued dialogue: " << dq.dialogueKey << std::endl;
#endif
        if (dialogueLines.find(dq.dialogueKey) == dialogueLines.end()) {
#ifdef DEBUG_DIALOGUE
            std::cout << "[DialogueHUD] Could not find dialogue: " << dq.dialogueKey << std::endl;
#endif
        } else {
            currentDialogue = dq.dialogueKey;
            dialogueEndFunc = dq.dialogueEndFunc;
            currentLine = 0;
            startLine = true;
#ifdef DEBUG_DIALOGUE
            std::cout << "[DialogueHUD] Starting dialogue: " << dq.dialogueKey << std::endl;
#endif
        }
        dialogueQueue.pop();
    }

    if (!currentDialogue.empty()) {
        CURRENT_STATE.openUI = true;
        textTimer.Update(dt);
        bool skipFirstKeyCheck = startLine;

        DialogueLine dialLine = dialogueLines.at(currentDialogue)[currentLine];
        if (startLine) {
#ifdef DEBUG_DIALOGUE
            std::cout << "[DialogueHUD] [" << currentDialogue << "] Starting line " << currentLine << std::endl;
#endif
            if (dialLine.picture == "HelenaAssustada") pictureSprite = &helenaAssustada;
            else if (dialLine.picture == "HelenaBrava") pictureSprite = &helenaBrava;
            else if (dialLine.picture == "HelenaFeliz") pictureSprite = &helenaFeliz;
            else if (dialLine.picture == "HelenaNeutra") pictureSprite = &helenaNeutra;
            else if (dialLine.picture == "Madu") pictureSprite = &madu;
            else if (dialLine.picture == "Maju") pictureSprite = &maju;
            else if (dialLine.picture == "Malu") pictureSprite = &malu;
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

        if ((INPUT_MANAGER.KeyPress(SDLK_RETURN) || INPUT_MANAGER.IsKeyDown(SDLK_SPACE) || INPUT_MANAGER.CButtonPress(SDL_CONTROLLER_BUTTON_A)) && !skipFirstKeyCheck) {
            if (picturePos.x != 0) picturePos.x = 0;
            else if (currentTextPos < dialLine.line.length()) currentTextPos = dialLine.line.length();
            else {
                currentLine++;
                if (currentLine >= dialogueLines.at(currentDialogue).size()) {
                    CURRENT_STATE.openUI = false;
                    currentDialogue = "";
                    if (dialogueEndFunc != nullptr) dialogueEndFunc();
                } else startLine = true;
            }
        }
    }
}

void DialogueHUD::Render() {
    if (!currentDialogue.empty()) {
        SDL_Rect screenRect = {0, 0, 1200, 900};
        SDL_SetRenderDrawBlendMode(GAME_RENDERER, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(GAME_RENDERER, 0, 0, 0, 127);
        SDL_RenderFillRect(GAME_RENDERER, &screenRect);

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

void DialogueHUD::RequestDialogue(std::string dialogueKey, std::function<void ()> func) {
#ifdef DEBUG_DIALOGUE
    std::cout << "[DialogueHUD] Requesting dialogue: " << dialogueKey << std::endl;
#endif
    dialogueQueue.push(DialogueQuery(dialogueKey, func));
}
