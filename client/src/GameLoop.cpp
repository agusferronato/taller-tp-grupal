#include "GameLoop.h"




GameLoop::GameLoop(Queue<Command> &receptionQueue, Queue<Command> &sendingQueue) 
    : receptionQueue(receptionQueue),
      sendingQueue(sendingQueue) { }

      
void GameLoop::run(unsigned int it)
{


}




void GameLoop::initSDL()
{
}
