#ifndef CONSTANT_RATELOOP_H
#define CONSTANT_RATELOOP_H

#include <chrono>
#include "Constants.h"
#include <cmath>
#include <thread>
#include "thread.h"

class ConstantRateLoop {
public:
    ConstantRateLoop(int framesPerSecond);
    virtual ~ConstantRateLoop() = default;

    virtual void updateTimer(unsigned int &it);

private:
    int rateInMiliseconds;
    std::chrono::time_point<std::chrono::steady_clock> t0;
};

#endif
