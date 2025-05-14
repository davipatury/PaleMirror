#ifndef TEXT_H
#define TEXT_H

#include "SDL_ttf.h"

#include <iostream>
#include <string>

#include "Camera.h"
#include "Component.h"
#include "Resources.h"
#include "Timer.h"

class Text : public Component
{
public:
    enum TextStyle { SOLID, SHADED, BLENDED };
    Text(GameObject& associated, const char* fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, float flashingTime = 0);
    ~Text();

    void Update(float dt);
    void Render();
    bool Is(std::string type);

    void SetText(std::string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(const char* fontFile);
    void SetFontSize(int fontSize);
private:
    void RemakeTexture();
    TTF_Font* font;
    SDL_Texture* texture;

    std::string text;
    TextStyle style;
    const char* fontFile;
    int fontSize;
    SDL_Color color;
    float flashingTime;
    Timer flashTimer;
    bool hiding;
};

#endif // TEXT_H
