#ifndef SPRITE_H
#define SPRITE_H

#include <iostream>
#include <string>

#include "SDL_image.h"

#include "components/Camera.h"
#include "utils/Resources.h"
#include "math/Vec2.h"

class Sprite
{
public:
    Sprite();
    Sprite(const char* file, int frameCountW = 1, int frameCountH = 1);
    ~Sprite();

    void Open(const char* file);
    void SetClip(int x, int y, int w, int h);
    void Render(int x, int y, int w, int h, float angle = 0);
    void SetFrame(int frame);
    void SetFrameCount(int frameCountW, int frameCountH);
    void SetCameraFollower(bool newValue);

    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();
    void SetFlip(SDL_RendererFlip flip);

    int GetWidth();
    int GetHeight();
    bool IsOpen();

    SDL_Texture* texture;
private:

    int width;
    int height;
    int frameCountW;
    int frameCountH;
    SDL_Rect clipRect;
    bool cameraFollower;

    SDL_RendererFlip flip;
    Vec2 scale;
};

#endif // SPRITE_H
