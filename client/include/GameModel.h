#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "GameWindow.h"
#include "Player.h"
#include "Queue.h"
#include <memory>
#include <unordered_map>

class GameModel {
private:
  Queue<ServerEventDTO> &receptionQueue;
  Queue<ClientCommandDTO> &sendingQueue;
  uint32_t myPlayerID;
  std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
  GameWindow *gameView;

public:
  GameModel(uint32_t myPlayerID, GameWindow *gameView,
            Queue<ServerEventDTO> &receptionQueue,
            Queue<ClientCommandDTO> &sendingQueue);
  void updateStateFromServer();

public:
  /*update State From Controller*/
  void moveMyPlayer(Direction direction);
  void stopMyPlayer();

private:
  void registerPlayers();

private:
  /* Event handlers */
  void playerAppeared(const ServerEventDTO &event);
  void playerMovedHandler(const ServerEventDTO &event);
  void playerStopped(const ServerEventDTO &event);
  void handleTextureInfo(const ServerEventDTO &event);
};

#endif
