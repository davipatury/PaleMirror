#ifndef SOUND_H
#define SOUND_H

#include <iostream>
#include <string>

#include "SDL_mixer.h"

#include "utils/Resources.h"

class Sound
{
public:
    Sound();
    Sound(const char* file);
    ~Sound();

    void Play(int time = 1);
    void Stop();
    void Open(const char* file);

    bool IsOpen();
private:
    Mix_Chunk* chunk;
    int channel;
};

#endif // SOUND_H
