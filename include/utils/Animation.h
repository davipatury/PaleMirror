#ifndef ANIMATION_H
#define ANIMATION_H

#include "SDL_image.h"

class Animation
{
public:
    Animation(int frameStart, int frameEnd, float frameTime, SDL_RendererFlip flip = SDL_FLIP_NONE);
    int frameStart;
    int frameEnd;
    float frameTime;
    SDL_RendererFlip flip;
};

#endif // ANIMATION_H
