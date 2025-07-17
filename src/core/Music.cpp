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

void Music::FadeInPos(double pos, int times, int ms) {
    if (music != nullptr) {
        Mix_FadeInMusicPos(music, times, ms, pos);
    }
}

void Music::SetPosition(double pos) {
    if (music != nullptr && Mix_PlayingMusic()) {
        Mix_SetMusicPosition(pos);
    }
}

double Music::GetPosition() {
    if (music != nullptr && Mix_PlayingMusic()) {
        return Mix_GetMusicPosition(music);
    }
    return 0.0d;
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
