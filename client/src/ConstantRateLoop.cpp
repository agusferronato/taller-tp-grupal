#include "ConstantRateLoop.h"

ConstantRateLoop::ConstantRateLoop(int framesPerSecond)
{
    this->t0 = std::chrono::steady_clock::now();
    rateInMiliseconds = 1000 / framesPerSecond;
}

void ConstantRateLoop::updateTimer(unsigned int &it)
{
    auto t1 = std::chrono::steady_clock::now();
    auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    auto rateDuration = std::chrono::milliseconds(this->rateInMiliseconds);
    auto rest = rateDuration - delta;

    if (rest < std::chrono::milliseconds(0)) {

        auto behind = -rest;
        rest = rateDuration - std::chrono::milliseconds(behind.count() % this->rateInMiliseconds);
        auto lost = behind + rest;

        t1 += lost;
        it += static_cast<unsigned int>(lost.count() / this->rateInMiliseconds);
    }

    std::this_thread::sleep_for(rest);
    t1 += rateDuration;
    it += 1;
}
