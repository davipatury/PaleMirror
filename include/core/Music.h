#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <string>

#include "SDL_mixer.h"

#include "utils/Resources.h"

class Music
{
public:
    Music();
    Music(const char* file);
    ~Music();

    void Play(int times = -1);
    void Stop(int msToStop = 1500);
    void Open(const char* file);

    void SetPosition(double pos);
    void FadeInPos(double pos, int times = -1, int ms = 2500);
    double GetPosition();

    void Pause();
    void Resume();

    bool IsOpen();
private:
    Mix_Music* music;
};

#endif // MUSIC_H
