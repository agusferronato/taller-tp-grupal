#ifndef GAMELOOP_H
#define GAMELOOP_h


#include <queue.h>
#include <Command.h>


class GameLoop {


private:
    Queue<Command>& receptionQueue;
    Queue<Command>& sendingQueue;

public:

    GameLoop(Queue<Command>& receptionQueue, Queue<Command>& sendingQueue); 

    void run(unsigned int it);

private:

    void initSDL();

};





#endif