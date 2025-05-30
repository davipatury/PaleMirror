#include "core/Music.h"

Music::Music() {
    music = nullptr;
}

Music::Music(const char* file): Music() {
    Open(file);
}

Music::~Music() {
    Stop();
}

void Music::Play(int times) {
    if (music != nullptr) {
        Mix_PlayMusic(music, times);
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop);
}

void Music::Open(const char* file) {
    music = Resources::GetMusic(file);
}

bool Music::IsOpen() {
    return music != nullptr;
}
