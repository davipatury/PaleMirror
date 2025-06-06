#include "utils/Timer.h"

Timer::Timer() {
    time = 0;
}

void Timer::Update(float dt) {
    time += dt;
}

void Timer::Restart() {
    time = 0;
}

float Timer::Get() {
    return time;
}
