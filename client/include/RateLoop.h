#ifndef RATE_LOOP_H
#define RATE_LOOP_H

#include <bits/chrono.h>
#include "GameLoop.h"
#include "Constants.h"
#include <cmath>
#include <thread>

class RateLoop {

private:
    GameLoop gameloop;
    bool isRunning = true;
    int rate;


public:

    RateLoop();


    void run();

};





#endif