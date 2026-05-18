#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <list>
#include <unordered_map>
#include <thread>
#include <utility>
#include <memory>

#include "queue.h"
#include "thread.h"
#include "SenderQueueMonitor.h"
#include "CommandDTO.h"
#include "ConstantRateLoop.h"
#include "Constants.h"
#include "direction.h"
#include "types.h"

struct PlayerInfo {
    int x{0}, y{0};
    Direction direction{Direction::Down};
};

class Game: public Thread {

private:
    Queue<std::unique_ptr<CommandDTO>>& gameloopQueue;
    SenderQueueMonitor& senderQueueMonitor;

    std::list<std::unique_ptr<CommandDTO>> messagesToSend;
    bool keepRunning = true;
    PlayerId nextPlayerId{1};
    std::unordered_map<PlayerId, PlayerInfo> players;

public:
    Game(Queue<std::unique_ptr<CommandDTO>>& gameloopQueue, SenderQueueMonitor& senderQueueMonitor);

    virtual void run() override;

    void kill();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

private:
    void execute(std::unique_ptr<CommandDTO> clientMessage);
    void sendMessages();
};

#endif
