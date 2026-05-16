#ifndef GAMELOOP_H
#define GAMELOOP_H


#include <queue.h>
#include <Command.h>
#include "ShutdownEvent.h"

class GameLoop {

private:
    Queue<Command>& receptionQueue;
    Queue<Command>& sendingQueue;
    ShutdownEvent& shutdownEvent;

public:

    GameLoop(
        Queue<Command>& receptionQueue,
        Queue<Command>& sendingQueue,
        ShutdownEvent& shutdownEvent
    );

    void run(unsigned int it);

private:

    void initSDL();

};

#endif
