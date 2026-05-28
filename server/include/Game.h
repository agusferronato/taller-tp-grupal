#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <list>
#include <memory>
#include <thread>
#include <unordered_map>
#include <utility>

#include "ClientMessage.h"
#include "ConstantRateLoop.h"
#include "Constants.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "MapData.h"
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
  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;

  std::list<ServerEventDTO> messagesToSend;
  bool keepRunning = true;
  std::unordered_map<uint32_t, PlayerInfo> players;

  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::list<TileOrigin> textureOrigins;

public:
  Game(Queue<ClientMessage> &gameloopQueue,
       SenderQueueMonitor &senderQueueMonitor);

  virtual void run() override;

  void kill();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void registerPlayer(uint32_t connectionId);
  void movePlayer(uint32_t playerId, Direction direction);
  void stopPlayer(uint32_t playerId);

private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
};

#endif
