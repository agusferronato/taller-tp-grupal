#include <algorithm>
#include <cctype>
#include <cmath>

#include "CityEntityAppearedEventDTO.h"
#include "CityEntityMovedEventDTO.h"
#include "CityEntityStoppedEventDTO.h"
#include "ConstantRateLoop.h"
#include "Formulas.h"
#include "Game.h"
#include "GlobalChatMessageEventDTO.h"
#include "GroundItemsListEventDTO.h"
#include "InventoryUpdateEventDTO.h"
#include "MapLoader.h"
#include "MoveCommandDTO.h"
#include "NPCAppearedEventDTO.h"
#include "NPCMovedEventDTO.h"
#include "NPCStoppedEventDTO.h"
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
#include <NPCType.h>

Game::Game(Queue<ClientMessage> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository,
           const std::string &mapPath)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor),
      repository(repository), mapPath(mapPath) {}

void Game::run() {
  MapLoader mapLoader(mapPath);
  maxSize = mapLoader.GetMaxSize();
  gridSize = mapLoader.GetGridSize();
  commonGroundTextureId = mapLoader.GetCommonGroundTextureId();
  textureOrigins = mapLoader.GetTextureOrigins();
  collidableCells = mapLoader.GetCollidableCells();

  biomes = std::move(mapLoader.GetBiomes());
  cities = std::move(mapLoader.GetCities());
  createCityEntities();

  for (uint8_t id = 1; id <= 19; ++id) {
    inventoryManager.addGroundItem(id, (id - 1) * 64, 32);
  }

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

    makeNPCsfollowPlayers();
    makeCitiesEntitiesFollowPlayers();
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
  player->addItem(17);
  player->addItem(1);
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

  for (auto &npc : npcs) {
    senderQueueMonitor.sendToClient(
        connectionId,
        NPCAppearedEventDTO{npc->getId(), static_cast<uint8_t>(npc->getType()),
                            static_cast<int16_t>(npc->getX()),
                            static_cast<int16_t>(npc->getY())});
  }

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      senderQueueMonitor.sendToClient(
          connectionId,
          CityEntityAppearedEventDTO{
              entity->getId(),
              static_cast<uint8_t>(entity->getCityEntityType()),
              static_cast<int16_t>(entity->getX()),
              static_cast<int16_t>(entity->getY()), entity->getDirection()});
    }
  }

  messagesToSend.push_back(players[newId]->toPlayerAppeared());

  messagesToSend.push_back(InventoryUpdateEventDTO{
      newId, players[newId]->getInventoryItems(),
      players[newId]->getEquippedWeapon(), players[newId]->getEquippedArmor(),
      players[newId]->getEquippedHelmet(),
      players[newId]->getEquippedShield()});

  {
    std::vector<GroundItemInfoDTO> groundItemList;
    for (const auto &gi : inventoryManager.getGroundItems()) {
      groundItemList.push_back({gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                static_cast<int16_t>(gi.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId, GroundItemsListEventDTO{std::move(groundItemList)});
  }
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

  for (auto &npc : npcs) {
    senderQueueMonitor.sendToClient(
        connectionId,
        NPCAppearedEventDTO{npc->getId(), static_cast<uint8_t>(npc->getType()),
                            static_cast<int16_t>(npc->getX()),
                            static_cast<int16_t>(npc->getY())});
  }

  for (auto &city : cities) {
    for (auto *entity : city.getEntities()) {
      senderQueueMonitor.sendToClient(
          connectionId,
          CityEntityAppearedEventDTO{
              entity->getId(),
              static_cast<uint8_t>(entity->getCityEntityType()),
              static_cast<int16_t>(entity->getX()),
              static_cast<int16_t>(entity->getY()), entity->getDirection()});
    }
  }

  messagesToSend.push_back(players[newId]->toPlayerAppeared());

  messagesToSend.push_back(InventoryUpdateEventDTO{
      newId, players[newId]->getInventoryItems(),
      players[newId]->getEquippedWeapon(), players[newId]->getEquippedArmor(),
      players[newId]->getEquippedHelmet(),
      players[newId]->getEquippedShield()});

  {
    std::vector<GroundItemInfoDTO> groundItemList;
    for (const auto &gi : inventoryManager.getGroundItems()) {
      groundItemList.push_back({gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                static_cast<int16_t>(gi.y)});
    }
    senderQueueMonitor.sendToClient(
        connectionId, GroundItemsListEventDTO{std::move(groundItemList)});
  }
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
  inventoryManager.equipItem(playerId, inventorySlot);
}

void Game::unequipSlot(uint32_t playerId, uint8_t equipSlot) {
  inventoryManager.unequipSlot(playerId, static_cast<EquipSlot>(equipSlot));
}

void Game::dropItem(uint32_t playerId, uint8_t inventorySlot) {
  inventoryManager.dropItem(playerId, inventorySlot);
}

void Game::takeItem(uint32_t playerId) { inventoryManager.takeItem(playerId); }

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

  return false;
}

void Game::appearNPC(std::unique_ptr<NPC> &&npc) {
  int center = maxSize / 2;
  int px = (npc->getPosition().row - center) * gridSize;
  int py = (npc->getPosition().column - center) * gridSize;
  npc->setPixelPosition(px, py);

  uint32_t id = nextNPCId++;
  npc->setId(id);

  colisionables.push_back(npc.get());

  messagesToSend.push_back(
      NPCAppearedEventDTO{id, static_cast<uint8_t>(npc->getType()),
                          static_cast<int16_t>(px), static_cast<int16_t>(py)});

  npcs.push_back(std::move(npc));
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info->isMoving())
      continue;

    auto [targetX, targetY] = info->getTargetPosition();

    int origX = info->getX();
    int origY = info->getY();

    info->move(targetX, targetY);

    if (checkIfItCollides(info.get())) {
      info->move(origX, origY);
      continue;
    }

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
  std::string senderName = std::to_string(playerId);
  auto it = players.find(playerId);
  if (it != players.end()) {
    senderName = it->second->getName();
  }

  messagesToSend.push_back(
      GlobalChatMessageEventDTO{std::move(senderName), message});
}

void Game::atack(uint32_t playerId, int16_t x, int16_t y) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  auto atacker = it->second.get();
  if (!atacker->assertAtackDistance(x, y)) {
    return;
  }
  auto targetNPC = findNPCByCoordinates(x, y);
  if (targetNPC != nullptr) {
    playerAtackNPC(*atacker, *targetNPC);
    return;
  }

  auto targetPlayer = findPlayerByCoordinates(x, y);
  if (targetPlayer != nullptr) {
    playerAtackPlayer(*atacker, *targetPlayer);
    return;
  }
}

void Game::appearNPCs() {
  for (auto &biome : biomes) {
    biome->NPCgenerationStrategy(*this);
  }
}

bool Game::checkIfItCollides(Colisionable *entity) {
  for (auto &col : colisionables) {
    if (col == entity)
      continue;
    if (col->colisionaCon(entity->getX(), entity->getY(), entity->getAncho(),
                          entity->getAlto()))
      return true;
  }

  int start_i = floorDiv(entity->getX(), gridSize) + maxSize / 2;
  int end_i =
      floorDiv(entity->getX() + entity->getAncho(), gridSize) + maxSize / 2;
  int start_j = floorDiv(entity->getY(), gridSize) + maxSize / 2;
  int end_j =
      floorDiv(entity->getY() + entity->getAlto(), gridSize) + maxSize / 2;
  for (int i = start_i; i <= end_i; i++) {
    for (int j = start_j; j <= end_j; j++) {
      if (collidableCells.count({i, j, 0}))
        return true;
    }
  }

  return false;
}

void Game::createCityEntities() {
  for (auto &city : cities) {
    city.createEntities(*this);

    for (auto *entity : city.getEntities()) {
      int center = maxSize / 2;
      int px = (entity->getPosition().row - center) * gridSize;
      int py = (entity->getPosition().column - center) * gridSize;
      entity->setPixelPosition(px, py);

      uint32_t id = nextCityEntityId++;
      entity->setId(id);
      colisionables.push_back(entity);

      messagesToSend.push_back(CityEntityAppearedEventDTO{
          id, static_cast<uint8_t>(entity->getCityEntityType()),
          static_cast<int16_t>(px), static_cast<int16_t>(py),
          entity->getDirection()});
    }
  }
}

void Game::makeNPCsfollowPlayers() {
  for (auto &npc : npcs) {
    Character *target = nullptr;

    for (auto &[_, player] : players) {
      if (player->isInCity(cities, gridSize, maxSize))
        continue;

      int dx = npc->getX() - player->getX();
      int dy = npc->getY() - player->getY();
      if (abs(dx) <= npc->getRange() && abs(dy) <= npc->getRange()) {
        target = player.get();
        break;
      }
    }

    if (target) {
      int oldX = npc->getX();
      int oldY = npc->getY();

      if (npc->updatePosition(*target)) {
        if (checkIfItCollides(npc.get())) {
          npc->setPixelPosition(oldX, oldY);
          npc->stop();
          messagesToSend.push_back(NPCStoppedEventDTO{npc->getId()});
        } else {
          messagesToSend.push_back(NPCMovedEventDTO{
              npc->getId(), static_cast<int16_t>(npc->getX()),
              static_cast<int16_t>(npc->getY()), npc->getDirection()});
        }
      }
    } else {
      if (npc->getIsMoving()) {
        npc->stop();
        messagesToSend.push_back(NPCStoppedEventDTO{npc->getId()});
      }
    }
  }
}

void Game::makeCitiesEntitiesFollowPlayers() {
  for (auto &city : cities) {
    for (auto *cityEntity : city.getEntities()) {
      Character *target = nullptr;

      for (auto &[_, player] : players) {
        if (!city.contains(player->getX(), player->getY(), gridSize, maxSize))
          continue;

        int dx = cityEntity->getX() - player->getX();
        int dy = cityEntity->getY() - player->getY();
        if (abs(dx) <= cityEntity->getRange() &&
            abs(dy) <= cityEntity->getRange()) {
          target = player.get();
          break;
        }
      }

      if (target) {
        int oldX = cityEntity->getX();
        int oldY = cityEntity->getY();

        if (cityEntity->updatePosition(*target)) {
          if (checkIfItCollides(cityEntity)) {
            cityEntity->setPixelPosition(oldX, oldY);
            cityEntity->stop();
            messagesToSend.push_back(
                CityEntityStoppedEventDTO{cityEntity->getId()});
          } else {
            messagesToSend.push_back(CityEntityMovedEventDTO{
                cityEntity->getId(), static_cast<int16_t>(cityEntity->getX()),
                static_cast<int16_t>(cityEntity->getY()),
                cityEntity->getDirection()});
          }
        }
      } else {
        if (cityEntity->getIsMoving()) {
          cityEntity->stop();
          messagesToSend.push_back(
              CityEntityStoppedEventDTO{cityEntity->getId()});
        }
      }
    }
  }
}

void Game::playerAtackPlayer(Character &atacker, Character &target) {
  if (!validAtack(atacker, target)) {
    return;
  }
  uint32_t damage = calculateDamage(atacker);
  bool critico = (damage != atacker.getDamage());

  if (!critico && target.tryParry()) {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{"Sistema", "Atacaste a " + target.getName() +
                                           " pero el lo esquivo"});
    senderQueueMonitor.sendToClient(
        playerToConnection[target.getId()],
        ChatMessageEventDTO{"Sistema",
                            atacker.getName() +
                                " trato de atacarte pero lo esquivaste"});
    return;
  }
  damage = target.takeDamage(damage);

  uint32_t xp = Formulas::calcularExperiencia(damage, atacker.getLevel(),
                                              target.getLevel());
  atacker.gainExperience(xp);

  if (target.getHp() == 0) {
    uint32_t oro = target.dropGoldOnDeath();
    atacker.addGold(oro);
    uint32_t xpMuerte = Formulas::calcularExperienciaMuerte(
        target.getMaxHp(), atacker.getLevel(), target.getLevel(),
        (std::rand() % 100) / 100.0);
    atacker.gainExperience(xpMuerte);
    // [TODO] volver fanstasma el target
    // [TODO] soltar items
  } else {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{
            "Sistema", "Atacaste a " + target.getName() + " y le hiciste " +
                           std::to_string(damage) + " de daño!"});
    senderQueueMonitor.sendToClient(
        playerToConnection[target.getId()],
        ChatMessageEventDTO{"Sistema",
                            "Recibiste un ataque de " + atacker.getName() +
                                " y te hicieron " + std::to_string(damage) +
                                " de daño!"});
  }
  messagesToSend.push_back(atacker.toPlayerInfoEvent());
  messagesToSend.push_back(target.toPlayerInfoEvent());
}

void Game::playerAtackNPC(Character &atacker, NPC &target) {
  uint32_t damage = calculateDamage(atacker);
  // [TODO] aplicar daño al npc
  /* [TODO] exp del ataque*/
  if (false) {
    // [TODO] muerte del npc
  } else {
    senderQueueMonitor.sendToClient(
        playerToConnection[atacker.getId()],
        ChatMessageEventDTO{
            "Sistema", "Atacaste a un " + target.getName() + " y le hiciste " +
                           std::to_string(damage) + " de daño!"});
  }
  messagesToSend.push_back(atacker.toPlayerInfoEvent());
}

uint32_t Game::calculateDamage(Character &atacker) {
  uint32_t damage = atacker.getDamage();
  if (Formulas::calcularCritico(std::rand())) {
    return damage * 2;
  }
  return damage;
}

bool Game::validAtack(Character &atacker, Character &target) {
  if (atacker.isNewbie() || target.isNewbie()) {
    return false;
  }
  if (abs(static_cast<int>(atacker.getLevel()) -
          static_cast<int>(target.getLevel())) > 10) {
    return false;
  }
  for (const auto &city : cities) {
    if (city.contains(atacker.getX(), atacker.getY(), gridSize, maxSize) ||
        city.contains(target.getX(), target.getY(), gridSize, maxSize)) {
      return false;
    }
  }
  return true;
}

Character *Game::findPlayerByCoordinates(int16_t x, int16_t y) {
  for (auto &[pid, player] : players) {
    if (player->colisionaCon(x, y, 16, 16)) {
      return player.get();
    }
  }
  return nullptr;
}

NPC *Game::findNPCByCoordinates(int16_t x, int16_t y) {
  for (auto &npc : npcs) {
    if (npc->colisionaCon(x, y, 16, 16)) {
      return npc.get();
    }
  }
  return nullptr;
}
