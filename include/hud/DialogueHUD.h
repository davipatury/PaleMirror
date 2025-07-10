#ifndef DIALOGUEHUD_H
#define DIALOGUEHUD_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <functional>

#include "components/Camera.h"
#include "entities/characters/Character.h"
#include "core/Component.h"
#include "utils/TextHUD.h"

class DialogueHUD : public Component
{
public:
    DialogueHUD(GameObject& associated);
    ~DialogueHUD();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    static void RequestDialogue(std::string dialogueKey, std::function<void ()> func = nullptr);
private:
    class DialogueLine {
    public:
        DialogueLine(std::string picture, std::string charName, SDL_Color charNameColor, std::string line, bool resetPos = false) {
            this->picture = picture;
            this->charName = charName;
            this->charNameColor = charNameColor;
            this->line = line;
            this->resetPos = resetPos;
        }
        std::string picture;
        std::string charName;
        SDL_Color charNameColor;
        std::string line;
        bool resetPos;
    };

    class DialogueQuery {
    public:
        DialogueQuery(std::string dialogueKey, std::function<void ()> func = nullptr) {
            this->dialogueKey = dialogueKey;
            this->dialogueEndFunc = func;
        }
        std::string dialogueKey;
        std::function<void ()> dialogueEndFunc;
    };

    int currentTextPos;
    Timer textTimer;

    TextHUD* dialogueText;
    TextHUD* charNameText;

    Sprite caixaDialogo, helenaAssustada, helenaBrava, helenaFeliz, helenaNeutra;

    Sprite* pictureSprite;
    Vec2 picturePos;

    std::unordered_map<std::string, std::vector<DialogueLine>> dialogueLines;

    std::string currentDialogue;
    std::function<void ()> dialogueEndFunc;
    int currentLine;
    bool startLine;
    static std::queue<DialogueHUD::DialogueQuery> dialogueQueue;
};

#endif // DIALOGUEHUD_H
