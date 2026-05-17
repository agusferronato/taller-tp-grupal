#ifndef GAME_H
#define GAME_H


#include <chrono>
#include <list>
#include <thread>
#include <utility>

#include "queue.h"
#include "thread.h"
#include "SenderQueueMonitor.h"
#include <Command.h>
#include "ConstantRateLoop.h"
#include "Constants.h"


class Game: public Thread {

private:
    Queue<Command>& gameloopQueue;
    SenderQueueMonitor& senderQueueMonitor;

    std::list<Command> messagesToSend;
    bool keepRunning = true;

public:
    Game(Queue<Command>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor);

    virtual void run() override;

    void kill();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

private:
    void execute(Command& clientMessage);
    void sendMessages();

};

#endif