#include "utils/TextHUD.h"

#include "core/Game.h"

TextHUD::TextHUD(Vec2 pos, const char* fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, int wrapLength, float flashingTime) {
    this->pos = pos;
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->style = style;
    this->text = text;
    this->color = color;
    this->flashingTime = flashingTime;
    this->wrapLength = wrapLength;
    hiding = false;
    texture = nullptr;

    font = Resources::GetFont(fontFile, fontSize);
    RemakeTexture();
}

TextHUD::~TextHUD() {
    if (texture != nullptr) SDL_DestroyTexture(texture);
}

void TextHUD::Update(float dt) {
    if (flashingTime > 0) {
        flashTimer.Update(dt);
        if (flashTimer.Get() > flashingTime) {
            hiding = !hiding;
            flashTimer.Restart();
        }
    }
}

void TextHUD::Render() {
    if (texture == nullptr || hiding || text.empty()) return;
    //if (hiding) return;
    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = rect.w;
    srcrect.h = rect.h;
    SDL_Rect dstrect;
    dstrect.x = pos.x;
    dstrect.y = pos.y;
    dstrect.w = rect.w;
    dstrect.h = rect.h;
    SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &srcrect, &dstrect, 0, nullptr, SDL_FLIP_NONE);
}

void TextHUD::RemakeTexture() {
    if (texture != nullptr) SDL_DestroyTexture(texture);
    if (text.empty()) return;

    SDL_Surface* surface;
    switch (style) {
    case SOLID:
        surface = TTF_RenderText_Solid_Wrapped(font, text.c_str(), color, wrapLength);
        break;
    case SHADED:
        surface = TTF_RenderText_Shaded_Wrapped(font, text.c_str(), color, {0, 0, 0, 255}, wrapLength);
        break;
    case BLENDED:
        surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, wrapLength);
        break;
    }

    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surface);
    rect.w = surface->w;
    rect.h = surface->h;
    SDL_FreeSurface(surface);
}

void TextHUD::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void TextHUD::SetFontFile(const char *fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void TextHUD::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void TextHUD::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void TextHUD::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}
