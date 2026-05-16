#include "GameLoop.h"


GameLoop::GameLoop(
    Queue<Command>& receptionQueue,
    Queue<Command>& sendingQueue,
    ShutdownEvent& shutdownEvent
) : receptionQueue(receptionQueue),
    sendingQueue(sendingQueue),
    shutdownEvent(shutdownEvent) { }


void GameLoop::run(unsigned int it)
{
  (void)it;
  try {
    std::unique_ptr<Command> command;

    if (receptionQueue.try_pop(*command)) {

    }

  } catch (const ClosedQueue& e) {

  }


}


void GameLoop::initSDL()
{
}
