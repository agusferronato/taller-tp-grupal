#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <list>
#include <memory>
#include <thread>
#include <unordered_map>
#include <utility>

#include "CommandDTO.h"
#include "ConstantRateLoop.h"
#include "Constants.h"
#include "Direction.h"
#include "Queue.h"
#include "SenderQueueMonitor.h"
#include "Thread.h"

struct PlayerInfo {
  int x{0}, y{0};
  Direction direction{Direction::Down};
  bool moving{false};
};

class Game : public Thread {

private:
  Queue<std::unique_ptr<CommandDTO>> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;

  std::list<std::unique_ptr<CommandDTO>> messagesToSend;
  bool keepRunning = true;
  uint32_t nextPlayerId{1};
  std::unordered_map<uint32_t, PlayerInfo> players;

public:
  Game(Queue<std::unique_ptr<CommandDTO>> &gameloopQueue,
       SenderQueueMonitor &senderQueueMonitor);

  virtual void run() override;

  void kill();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

private:
  void execute(std::unique_ptr<CommandDTO> clientMessage);
  void sendMessages();
  void movePlayers();
};

#endif
