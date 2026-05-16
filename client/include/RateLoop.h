#ifndef RATE_LOOP_H
#define RATE_LOOP_H

#include <bits/chrono.h>
#include "GameLoop.h"
#include "Constants.h"
#include <cmath>
#include <thread>
#include "thread.h"
#include "ShutdownEvent.h"

class RateLoop : public Thread {

private:
    GameLoop& game;
    ShutdownEvent& shutdownEvent;
    int rate;

public:

    RateLoop(GameLoop& game, ShutdownEvent& shutdownEvent);

    virtual void run() override;

};

#endif
