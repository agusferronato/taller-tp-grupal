#ifndef RATE_LOOP_H
#define RATE_LOOP_H

#include <bits/chrono.h>
#include "GameLoop.h"
#include "Constants.h"
#include <cmath>
#include <thread>
#include "thread.h"


class RateLoop : public Thread {

private:
    GameLoop& gameloop;
    bool isRunning = true;
    int rate;


public:

    RateLoop(GameLoop& gameloop);

    virtual void run() override;

};





#endif