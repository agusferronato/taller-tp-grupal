#include "GameModel.h"
#include "GameWindow.h"
#include "LoginPlayerDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerListDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerStoppedDTO.h"
#include "RegisterPlayerDTO.h"
#include <iostream>
#include <stdexcept>

GameModel::GameModel(uint32_t myPlayerID, GameWindow *gameView,
                     Queue<std::unique_ptr<CommandDTO>> &receptionQueue,
                     Queue<std::unique_ptr<CommandDTO>> &sendingQueue)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      myPlayerID(myPlayerID), gameView(gameView) {
  auto myPlayer = std::make_unique<Player>(this->myPlayerID, 0, 0);
  players[myPlayerID] = std::move(myPlayer);
  gameView->addPlayer(myPlayerID, players[myPlayerID]->getObserver());
  registerPlayers();
}

void GameModel::updateStateFromServer() {

  std::unique_ptr<CommandDTO> cmd;

  while (receptionQueue.try_pop(cmd)) {

    switch (static_cast<ServerOpcode>(cmd->getCode())) {

    case ServerOpcode::PlayerMoved: {
      playerMovedHandler(cmd);
      break;
    }

    case ServerOpcode::PlayerAppeared: {
      playerAppeared(cmd);
      break;
    }

    case ServerOpcode::PlayerStopped: {
      playerStopped(cmd);
      break;
    }

    default:
      break;
    }
  }
}

void GameModel::updateStateFromController(std::unique_ptr<CommandDTO> cmdDTO) {
  sendingQueue.push(std::move(cmdDTO));
}

void GameModel::playerMovedHandler(std::unique_ptr<CommandDTO> &cmd) {

  auto *moved = dynamic_cast<PlayerMovedEventDTO *>(cmd.get());
  if (!moved)
    return;

  uint32_t pid = moved->getPlayerId();
  int16_t x = moved->getX();
  int16_t y = moved->getY();
  Direction dir = moved->getDirection();

  auto it = players.find(pid);
  if (it != players.end()) {
    it->second->updateCoordinates(x, y, dir);
  }
}

void GameModel::playerStopped(std::unique_ptr<CommandDTO> &cmd) {

  auto *stopped = dynamic_cast<PlayerStoppedDTO *>(cmd.get());
  if (!stopped)
    return;

  uint32_t pid = stopped->getPlayerID();

  auto it = players.find(pid);
  if (it != players.end()) {
    it->second->stopMoving();
  }
}

void GameModel::playerAppeared(std::unique_ptr<CommandDTO> &cmd) {

  auto *appeared = dynamic_cast<PlayerAppearedEventDTO *>(cmd.get());
  if (!appeared)
    return;

  uint32_t pid = appeared->getPlayerId();
  if (pid == myPlayerID)
    return;

  auto player =
      std::make_unique<Player>(pid, appeared->getX(), appeared->getY());
  players[pid] = std::move(player);
  gameView->addPlayer(pid, players[pid]->getObserver());
}

void GameModel::registerPlayers() {
  auto cmd = receptionQueue.pop();
  auto *list = dynamic_cast<PlayerListDTO *>(cmd.get());
  if (list) {
    for (const auto &info : list->getPlayers()) {
      if (info.player_id == myPlayerID)
        continue;
      auto player = std::make_unique<Player>(info.player_id, info.x, info.y);
      players[info.player_id] = std::move(player);
      gameView->addPlayer(info.player_id,
                          players[info.player_id]->getObserver());
    }
  }
}
