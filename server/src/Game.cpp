#include <algorithm>
#include <cctype>

#include "ConstantRateLoop.h"
#include "Formulas.h"
#include "Game.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "PlayerAppearedEventDTO.h"
#include "PlayerInfoEventDTO.h"
#include "PlayerListEventDTO.h"
#include "PlayerMovedEventDTO.h"
#include "PlayerRemovedEventDTO.h"
#include "PlayerStopCommandDTO.h"
#include "PlayerStoppedEventDTO.h"
#include "RegisterPlayerCommandDTO.h"
#include "RegisterPlayerEventDTO.h"
#include "TextureInfoEventDTO.h"
#include "command/CommandFactory.h"

static int floorDiv(int a, int b) { return (a >= 0) ? a / b : (a - b + 1) / b; }

Game::Game(Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor),
      repository(repository) {}

void Game::run() {
  MapLoader mapLoader("map.toml");
  maxSize = mapLoader.GetMaxSize();
  gridSize = mapLoader.GetGridSize();
  commonGroundTextureId = mapLoader.GetCommonGroundTextureId();
  textureOrigins = mapLoader.GetTextureOrigins();
  collidableCells = mapLoader.GetCollidableCells();

  ConstantRateLoop rateloop(FPS_SERVER);
  CommandFactory factory;
  unsigned int it = 0;
  unsigned int saveCounter = 0;

  while (keepRunning) {
    ClientMessage msg;

    if (gameloopQueue.try_pop(msg)) {
      auto command = factory.create(msg.dto);
      command->execute(*this, msg.connectionId);
    }
    movePlayers();
    sendMessages();

    if (++saveCounter >= 300) {
      for (auto &[id, player] : players) {
        repository.save(player->name, player->toPlayerData());
      }
      saveCounter = 0;
    }

    if (it % 60 == 0) {
      for (auto &[pid, p] : players) {
        messagesToSend.push_back(
            PlayerInfoEventDTO{pid, p->hp, p->maxHp, p->mana, p->maxMana,
                               p->gold, p->level, p->experience});
      }
    }

    rateloop.updateTimer(it);
  }

  saveAllPlayers();
}

void Game::kill() { keepRunning = false; }

void Game::registerPlayer(const std::string &name, const Race race,
                          const PlayerClass playerClass,
                          uint32_t connectionId) {

  if (repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId,
                                    RegisterPlayerEventDTO{0, 1});
    return;
  }

  uint32_t newId = nextPlayerId++;
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 64;

  auto player =
      std::make_unique<Character>(newId, spawnX, spawnY, Direction::Down);
  player->name = name;
  player->race = race;
  player->playerClass = playerClass;

  player->initializeStats(race, playerClass);
  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(
      connectionId, RegisterPlayerEventDTO{newId, 0});

  {
    std::vector<TextureOriginDTO> origins;
    origins.reserve(textureOrigins.size());
    for (const auto &o : textureOrigins) {
      origins.push_back(
          {static_cast<uint8_t>(o.priority), static_cast<uint8_t>(o.texture_id),
           static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId,
        TextureInfoEventDTO{
            static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
            static_cast<uint8_t>(commonGroundTextureId), std::move(origins)});
  }

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(
        {pid, static_cast<int16_t>(info->x), static_cast<int16_t>(info->y),
         info->direction, info->race, info->playerClass, info->name,
         info->hp, info->maxHp, info->mana, info->maxMana, info->gold,
         info->level, info->experience});
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(PlayerAppearedEventDTO{
      newId, static_cast<int16_t>(spawnX), static_cast<int16_t>(spawnY),
      Direction::Down, race, playerClass, name, players[newId]->hp,
      players[newId]->maxHp, players[newId]->mana, players[newId]->maxMana,
      players[newId]->gold, players[newId]->level, players[newId]->experience});
}


void Game::loginPlayer(const std::string &name, uint32_t connectionId) {

  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId,
                                    RegisterPlayerEventDTO{0, 1});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->name == name) {
      senderQueueMonitor.sendToClient(
          connectionId, RegisterPlayerEventDTO{0, 2});
      return;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = nextPlayerId++;

  auto player = std::make_unique<Character>(
      newId, data.x, data.y, static_cast<Direction>(data.direction));
  player->fromPlayerData(data);

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(
      connectionId, RegisterPlayerEventDTO{newId, 0});

  std::vector<TextureOriginDTO> origins;
  origins.reserve(textureOrigins.size());
  for (const auto &o : textureOrigins) {
    origins.push_back({static_cast<uint8_t>(o.priority),
                       static_cast<uint8_t>(o.texture_id),
                       static_cast<uint16_t>(o.x), static_cast<uint16_t>(o.y)});
  }
  senderQueueMonitor.sendToClient(
      connectionId,
      TextureInfoEventDTO{
          static_cast<uint16_t>(maxSize), static_cast<uint16_t>(gridSize),
          static_cast<uint8_t>(commonGroundTextureId), std::move(origins)});

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back(
        {pid, static_cast<int16_t>(info->x), static_cast<int16_t>(info->y),
         info->direction, info->race, info->playerClass, info->name,
         info->hp, info->maxHp, info->mana, info->maxMana, info->gold,
         info->level, info->experience});
  }
  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(PlayerAppearedEventDTO{
      newId, static_cast<int16_t>(data.x), static_cast<int16_t>(data.y),
      static_cast<Direction>(data.direction), players[newId]->race,
      players[newId]->playerClass, players[newId]->name,
      players[newId]->hp, players[newId]->maxHp, players[newId]->mana,
      players[newId]->maxMana, players[newId]->gold, players[newId]->level,
      players[newId]->experience});
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end()) {
    return;
  }

  Character &player = *it->second;
  player.direction = direction;
  player.moving = true;
}

void Game::stopPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  Character &player = *it->second;
  player.moving = false;
  messagesToSend.push_back(PlayerStoppedEventDTO{playerId});
}

void Game::exitPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }

  repository.save(it->second->name, it->second->toPlayerData());

  colisionables.erase(
      std::remove(colisionables.begin(), colisionables.end(), it->second.get()),
      colisionables.end());

  messagesToSend.push_back(PlayerRemovedEventDTO{playerId});

  auto connIt = playerToConnection.find(playerId);
  if (connIt != playerToConnection.end()) {
    connectionToPlayer.erase(connIt->second);
    playerToConnection.erase(connIt);
  }

  players.erase(it);
}

void Game::exitPlayerByConnection(uint32_t connectionId) {
  auto it = connectionToPlayer.find(connectionId);
  if (it == connectionToPlayer.end()) {
    return;
  }
  exitPlayer(it->second);
}

void Game::equipItem(uint32_t playerId, uint8_t inventorySlot) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  it->second->inventory.equipItem(inventorySlot);
}

void Game::unequipSlot(uint32_t playerId, uint8_t equipSlot) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  it->second->inventory.unequipSlot(static_cast<EquipSlot>(equipSlot));
}

void Game::dropItem(uint32_t playerId, uint8_t inventorySlot) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  it->second->inventory.removeItem(inventorySlot);
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info->moving) {
      continue;
    }

    auto [targetX, targetY] = info->getTargetPosition(info->direction);

    bool blocked = false;
    for (auto &col : colisionables) {
      if (col == info.get())
        continue;
      if (col->colisionaCon(targetX, targetY, info->getAncho(),
                            info->getAlto())) {
        blocked = true;
        break;
      }
    }
    if (blocked)
      continue;

    {
      int start_i = floorDiv(targetX, gridSize) + maxSize / 2;
      int end_i =
          floorDiv(targetX + info->getAncho() - 1, gridSize) + maxSize / 2;
      int start_j = floorDiv(targetY, gridSize) + maxSize / 2;
      int end_j =
          floorDiv(targetY + info->getAlto() - 1, gridSize) + maxSize / 2;
      bool tileBlocked = false;
      for (int i = start_i; i <= end_i; i++) {
        for (int j = start_j; j <= end_j; j++) {
          if (collidableCells.count({i, j, 0})) {
            tileBlocked = true;
            break;
          }
        }
        if (tileBlocked)
          break;
      }
      if (tileBlocked)
        continue;
    }

    info->x = targetX;
    info->y = targetY;

    messagesToSend.push_back(
        PlayerMovedEventDTO{playerID, static_cast<int16_t>(info->x),
                            static_cast<int16_t>(info->y), info->direction});
  }
}

void Game::sendMessages() {
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
}

void Game::saveAllPlayers() {
  for (auto &[id, player] : players) {
    repository.save(player->name, player->toPlayerData());
  }
}
