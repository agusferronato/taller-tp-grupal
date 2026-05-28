#include "GameModel.h"
#include "GameWindow.h"
#include "InventoryUpdateEventDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerEventDTO.h"
#include <iostream>
#include <stdexcept>

#include "protocol/ProtocolCodes.h"
#include "protocol/ServerEventCodes.h"

GameModel::GameModel(uint32_t myPlayerID, GameWindow *gameView,
                     Queue<ServerEventDTO> &receptionQueue,
                     Queue<ClientCommandDTO> &sendingQueue,
                     const std::string &race)
    : receptionQueue(receptionQueue), sendingQueue(sendingQueue),
      myPlayerID(myPlayerID), gameView(gameView) {
  auto myPlayer = std::make_unique<Player>(this->myPlayerID, 0, 0);
  myPlayer->setRace(race);
  players[myPlayerID] = std::move(myPlayer);
  gameView->addPlayer(myPlayerID, players[myPlayerID].get());
  registerPlayers();
}

void GameModel::updateStateFromServer() {

  ServerEventDTO event;

  while (receptionQueue.try_pop(event)) {
    switch (static_cast<EventOpcode>(getCode(event))) {
    case EventOpcode::PlayerMovedEvent:
      playerMovedHandler(event);
      break;

    case EventOpcode::PlayerAppearedEvent:
      playerAppeared(event);
      break;

    case EventOpcode::PlayerStoppedEvent:
      playerStopped(event);
      break;

    case EventOpcode::PlayerRemovedEvent:
      playerRemoved(event);
      break;

    case EventOpcode::PlayerInfoEvent:
      playerInfoUpdated(event);
      break;

    case EventOpcode::InventoryUpdateEvent:
      playerInventoryUpdated(event);
      break;

    default:
      break;
    }
  }
}

void GameModel::moveMyPlayer(Direction direction) {
  sendingQueue.push(MoveCommandDTO{myPlayerID, direction});
}

void GameModel::stopMyPlayer() {
  sendingQueue.push(PlayerStopCommandDTO{myPlayerID});
}

void GameModel::playerMovedHandler(const ServerEventDTO &event) {

  const auto *moved = std::get_if<PlayerMovedEventDTO>(&event);
  if (!moved) {
    return;
  }

  uint32_t pid = moved->playerId;
  int16_t x = moved->x;
  int16_t y = moved->y;
  Direction dir = moved->direction;

  auto it = players.find(pid);
  if (it != players.end()) {
    it->second->updateCoordinates(x, y, dir);
  }
}

void GameModel::playerStopped(const ServerEventDTO &event) {
  const auto *stopped = std::get_if<PlayerStoppedEventDTO>(&event);
  if (!stopped) {
    return;
  }

  auto it = players.find(stopped->playerId);
  if (it != players.end()) {
    it->second->stopMoving();
  }
}

void GameModel::playerAppeared(const ServerEventDTO &event) {
  const auto *appeared = std::get_if<PlayerAppearedEventDTO>(&event);
  if (!appeared) {
    return;
  }

  uint32_t pid = appeared->playerId;

  auto applyStats = [&](Player *p) {
    p->setName(appeared->playerName);
    p->setHp(appeared->hp);
    p->setMaxHp(appeared->maxHp);
    p->setMana(appeared->mana);
    p->setMaxMana(appeared->maxMana);
    p->setGold(appeared->gold);
    p->setLevel(appeared->level);
    p->setExperience(appeared->experience);
  };

  if (pid == myPlayerID) {
    players[pid]->setCoordinates(appeared->x, appeared->y);
    players[pid]->setRace(appeared->race);
    applyStats(players[pid].get());
    gameView->addPlayer(pid, players[pid].get());
    return;
  }

  auto player = std::make_unique<Player>(pid, appeared->x, appeared->y);
  player->setRace(appeared->race);
  applyStats(player.get());
  players[pid] = std::move(player);
  gameView->addPlayer(pid, players[pid].get());
}

void GameModel::playerRemoved(const ServerEventDTO &event) {
  const auto *removed = std::get_if<PlayerRemovedEventDTO>(&event);
  if (!removed) {
    return;
  }
  players.erase(removed->playerId);
}

void GameModel::playerInfoUpdated(const ServerEventDTO &event) {
  const auto *info = std::get_if<PlayerInfoEventDTO>(&event);
  if (!info) {
    return;
  }
  auto it = players.find(info->playerId);
  if (it == players.end()) {
    return;
  }
  it->second->setHp(info->hp);
  it->second->setMaxHp(info->maxHp);
  it->second->setMana(info->mana);
  it->second->setMaxMana(info->maxMana);
  it->second->setGold(info->gold);
  it->second->setLevel(info->level);
  it->second->setExperience(info->experience);
}

void GameModel::registerPlayers() {
  auto event = receptionQueue.pop();
  // si se cierra el socket el hilo reciver cierra y lanza ClosedQueue
  // debloquenado este pop

  auto *list = std::get_if<PlayerListEventDTO>(&event);
  if (list) {
    for (const auto &info : list->players) {
      if (info.playerId == myPlayerID) {
        continue;
      }
      auto player = std::make_unique<Player>(info.playerId, info.x, info.y);
      player->setRace(info.race);
      player->setName(info.playerName);
      player->setHp(info.hp);
      player->setMaxHp(info.maxHp);
      player->setMana(info.mana);
      player->setMaxMana(info.maxMana);
      player->setGold(info.gold);
      player->setLevel(info.level);
      player->setExperience(info.experience);
      players[info.playerId] = std::move(player);
      gameView->addPlayer(info.playerId, players[info.playerId].get());
    }
  }
}

void GameModel::playerInventoryUpdated(const ServerEventDTO &event) {
  const auto *inv = std::get_if<InventoryUpdateEventDTO>(&event);
  if (!inv)
    return;
  if (inv->playerId != myPlayerID)
    return;
  auto it = players.find(inv->playerId);
  if (it == players.end())
    return;
  it->second->setInventory(inv->items);
  it->second->setEquippedWeapon(inv->equippedWeapon);
  it->second->setEquippedArmor(inv->equippedArmor);
  it->second->setEquippedHelmet(inv->equippedHelmet);
  it->second->setEquippedShield(inv->equippedShield);
}
