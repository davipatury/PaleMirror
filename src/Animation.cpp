#include "Animation.h"

Animation::Animation(int frameStart, int frameEnd, float frameTime, SDL_RendererFlip flip) {
    this->frameStart = frameStart;
    this->frameEnd = frameEnd;
    this->frameTime = frameTime;
    this->flip = flip;
}
