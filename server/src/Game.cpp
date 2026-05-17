#include "Game.h"

Game::Game(Queue<std::unique_ptr<CommandDTO>>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor):
        gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor) {}

void Game::run() {

    ConstantRateLoop rateloop(FPS_SERVER);
    unsigned int it = 0;

    while (keepRunning) {
        
        auto command = gameloopQueue.pop();
        execute(std::move(command));

        sendMessages();

        rateloop.updateTimer(it);
    }
}

void Game::kill() { keepRunning = false; }

void Game::execute(std::unique_ptr<CommandDTO> clientMessage) {
    (void)clientMessage;
}

void Game::sendMessages() {
}
