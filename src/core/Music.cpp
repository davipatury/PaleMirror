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

void Music::Pause() {
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) {
        Mix_PauseMusic();
    }
}

void Music::Resume() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    }
}
