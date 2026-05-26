#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "CommandDTO.h"
#include "GameWindow.h"
#include "Player.h"
#include "Queue.h"
#include <memory>
#include <unordered_map>

class GameModel {
private:
  Queue<std::unique_ptr<CommandDTO>> &receptionQueue;
  Queue<std::unique_ptr<CommandDTO>> &sendingQueue;
  uint32_t myPlayerID;
  std::unordered_map<uint32_t, std::unique_ptr<Player>> players;
  GameWindow *gameView;

public:
  GameModel(uint32_t myPlayerID, GameWindow *gameView,
            Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
            Queue<std::unique_ptr<CommandDTO>> &sendingQueue);
  void updateStateFromServer();
  void updateStateFromController(
      std::unique_ptr<CommandDTO> cmdDTO); // luego cambiar

private:
  void registerPlayers();
  /* Event handlers */
  void playerAppeared(std::unique_ptr<CommandDTO> &cmd);
  void playerMovedHandler(std::unique_ptr<CommandDTO> &cmd);
  void playerStopped(std::unique_ptr<CommandDTO> &cmd);
};

#endif
