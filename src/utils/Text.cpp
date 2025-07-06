#include "utils/Text.h"

#include "core/Game.h"

Text::Text(GameObject& associated, const char* fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, float flashingTime) : Component(associated) {
    this->fontFile = fontFile;
    this->fontSize = fontSize;
    this->style = style;
    this->text = text;
    this->color = color;
    this->flashingTime = flashingTime;
    hiding = false;
    texture = nullptr;

    font = Resources::GetFont(fontFile, fontSize);
    RemakeTexture();
}

Text::~Text() {
    if (texture != nullptr) SDL_DestroyTexture(texture);
}

void Text::Update(float dt) {
    if (flashingTime > 0) {
        flashTimer.Update(dt);
        if (flashTimer.Get() > flashingTime) {
            hiding = !hiding;
            flashTimer.Restart();
        }
    }
}

void Text::Render() {
    if (texture == nullptr || hiding) return;
    //if (hiding) return;
    SDL_Rect srcrect;
    srcrect.x = 0;
    srcrect.y = 0;
    srcrect.w = associated.box.w;
    srcrect.h = associated.box.h;
    SDL_Rect dstrect;
    dstrect.x = associated.box.x - Camera::pos.x;
    dstrect.y = associated.box.y - Camera::pos.y;
    dstrect.w = associated.box.w;
    dstrect.h = associated.box.h;
    SDL_RenderCopyEx(GAME_RENDERER, texture, &srcrect, &dstrect, associated.angleDeg, nullptr, SDL_FLIP_NONE);
}

void Text::RemakeTexture() {
    if (texture != nullptr) SDL_DestroyTexture(texture);

    SDL_Surface* surface;
    switch (style) {
    case SOLID:
        surface = TTF_RenderText_Solid(font, text.c_str(), color);
        break;
    case SHADED:
        surface = TTF_RenderText_Shaded(font, text.c_str(), color, {0, 0, 0, 255});
        break;
    case BLENDED:
        surface = TTF_RenderText_Blended(font, text.c_str(), color);
        break;
    }

    texture = SDL_CreateTextureFromSurface(GAME_RENDERER, surface);
    associated.box.w = surface->w;
    associated.box.h = surface->h;
    SDL_FreeSurface(surface);
}

void Text::SetColor(SDL_Color color) {
    this->color = color;
    RemakeTexture();
}

void Text::SetFontFile(const char *fontFile) {
    this->fontFile = fontFile;
    RemakeTexture();
}

void Text::SetFontSize(int fontSize) {
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::SetStyle(TextStyle style) {
    this->style = style;
    RemakeTexture();
}

void Text::SetText(std::string text) {
    this->text = text;
    RemakeTexture();
}

bool Text::Is(std::string type) {
    return type == "Text";
}
