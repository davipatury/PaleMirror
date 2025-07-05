#ifndef TEXTHUD_H
#define TEXTHUD_H

#include "SDL_ttf.h"

#include <iostream>
#include <string>

#include "components/Camera.h"
#include "utils/Resources.h"
#include "utils/Timer.h"

class TextHUD
{
public:
    enum TextStyle { SOLID, SHADED, BLENDED };
    TextHUD(Vec2 pos, const char* fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, int wrapLength = 0, float flashingTime = 0);
    ~TextHUD();

    void Update(float dt);
    void Render();

    void SetText(std::string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(const char* fontFile);
    void SetFontSize(int fontSize);
    void SetPos(Vec2 pos);

    float GetWidth();
    float GetHeight();

    Vec2 pos;
    Rect rect;
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
    int wrapLength;
};

#endif // TEXTHUD_H
