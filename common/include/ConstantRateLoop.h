#ifndef CONSTANT_RATELOOP_H
#define CONSTANT_RATELOOP_H

#include <bits/chrono.h>
#include "Gameloop.h"
#include "Constants.h"
#include <cmath>
#include <thread>
#include "thread.h"
#include "ShutdownEvent.h"

class ConstantRateLoop {

private:
    int rateInMiliseconds;
    std::chrono::time_point<std::chrono::steady_clock> t0;

public:

    ConstantRateLoop(int framesPerSecond);

    virtual void updateTimer(unsigned int &it);

};

#endif
