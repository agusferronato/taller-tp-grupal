#include "Game.h"

Game::Game(Queue<Command>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor):
        gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}


void Game::run() {

    while (keepRunning) {
        
        Command command = gameloopQueue.pop();
        execute(command);
        sendMessages();

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
