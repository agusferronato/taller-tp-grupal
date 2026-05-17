#include "Game.h"

Game::Game(Queue<Command>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor):
        gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}


void Game::run() {


    // init resources...
    ConstantRateLoop rateloop(FPS_SERVER);
    unsigned int it = 0;


    while (keepRunning) {
        
        // receive updates from clients...
        Command command = gameloopQueue.pop();
        
        // logic and physics...
        execute(command);

        // broadcast...
        sendMessages();

        rateloop.updateTimer(it);
    }
}

void Game::kill() { keepRunning = false; }

void Game::execute(Command &clientMessage)
{
    (void)clientMessage;
}

void Game::sendMessages()
{
}
