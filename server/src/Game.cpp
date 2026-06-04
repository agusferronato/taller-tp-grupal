#include <algorithm>
#include <cctype>

#include "ConstantRateLoop.h"
#include "Game.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "NPCAppearedEventDTO.h"
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

  biomes = std::move(mapLoader.GetBiomes());
  cities = mapLoader.GetCities();

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
    appearNPCs();
    sendMessages();

    if (++saveCounter >= 300) {
      for (auto &[id, player] : players) {
        repository.save(player->getName(), player->toPlayerData());
      }
      saveCounter = 0;
    }

    if (it % 60 == 0) {
      for (auto &[pid, p] : players) {
        messagesToSend.push_back(p->toPlayerInfoEvent());
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
    senderQueueMonitor.sendToClient(connectionId, RegisterPlayerEventDTO{0, 1});
    return;
  }

  uint32_t newId = nextPlayerId++;
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 64;

  auto player = std::make_unique<Character>(newId, name, race, playerClass,
                                            spawnX, spawnY, Direction::Down);
  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId, 0});

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
    playerList.push_back(info->toPlayerInfo(pid));
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(players[newId]->toPlayerAppeared());
}

void Game::loginPlayer(const std::string &name, uint32_t connectionId) {

  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId, RegisterPlayerEventDTO{0, 1});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->getName() == name) {
      senderQueueMonitor.sendToClient(connectionId,
                                      RegisterPlayerEventDTO{0, 2});
      return;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = nextPlayerId++;

  auto player = std::make_unique<Character>(newId, data);

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId, 0});

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
    playerList.push_back(info->toPlayerInfo(pid));
  }
  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(players[newId]->toPlayerAppeared());
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end()) {
    return;
  }

  it->second->setDirection(direction);
}

void Game::stopPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  it->second->stop();
  messagesToSend.push_back(PlayerStoppedEventDTO{playerId});
}

void Game::exitPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }

  repository.save(it->second->getName(), it->second->toPlayerData());

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
  it->second->equipItem(inventorySlot);
}

void Game::unequipSlot(uint32_t playerId, uint8_t equipSlot) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;
  it->second->unequipSlot(static_cast<EquipSlot>(equipSlot));
}

void Game::dropItem(uint32_t playerId, uint8_t inventorySlot) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  it->second->removeItem(inventorySlot);
}

bool Game::thereIsACollidableEntityAt(Position position) {
  int center = maxSize / 2;
  int cellX = (position.row - center) * gridSize;
  int cellY = (position.column - center) * gridSize;

  if (collidableCells.count({position.row, position.column, 0}))
    return true;

  for (auto &col : colisionables) {
    if (!(cellX + gridSize <= col->getX() ||
          cellX >= col->getX() + col->getAncho() ||
          cellY + gridSize <= col->getY() ||
          cellY >= col->getY() + col->getAlto()))
      return true;
  }

  for (auto &npc : npcs) {
    if (!(cellX + gridSize <= npc->getX() ||
          cellX >= npc->getX() + npc->getAncho() ||
          cellY + gridSize <= npc->getY() ||
          cellY >= npc->getY() + npc->getAlto()))
      return true;
  }

  return false;
}

void Game::appearNPC(std::unique_ptr<NPC> &&npc) {
  int center = maxSize / 2;
  int px = (npc->getPosition().row - center) * gridSize;
  int py = (npc->getPosition().column - center) * gridSize;
  npc->setPixelPosition(px, py);

  uint16_t id = nextNPCId++;
  messagesToSend.push_back(
      NPCAppearedEventDTO{id, static_cast<uint8_t>(npc->getType()),
                          static_cast<int16_t>(px), static_cast<int16_t>(py)});

  std::cout << "NPC of type " << (int)npc->getType() << std::endl;
  npcs.push_back(std::move(npc));
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info->isMoving()) {
      continue;
    }

    auto [targetX, targetY] = info->getTargetPosition();

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

    info->move(targetX, targetY);

    messagesToSend.push_back(info->toPlayerMoved());
  }
}

void Game::sendMessages() {
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
}

void Game::saveAllPlayers() {
  for (auto &[id, player] : players) {
    repository.save(player->getName(), player->toPlayerData());
  }
}

void Game::sendGlobalChatMessage(uint32_t playerId,
                                 const std::string &message) {
  messagesToSend.push_back(
      GlobalChatMessageEventDTO{std::to_string(playerId), message});
}

void Game::appearNPCs() {

  for (auto &biome : biomes) {
    biome->NPCgenerationStrategy(*this);
  }
}
