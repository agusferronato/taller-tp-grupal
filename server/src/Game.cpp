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

PlayerInfo::PlayerInfo(uint32_t id, int x, int y, Direction dir)
    : id(id), x(x), y(y), direction(dir) {}

PlayerData PlayerInfo::toPlayerData() const {
  PlayerData data{};
  data.setName(name);
  data.setPassword(password);
  data.setRace(RaceUtils::raceToString(race));
  data.setPlayerClass(playerClass);
  data.x = x;
  data.y = y;
  data.direction = static_cast<uint8_t>(direction);
  data.level = level;
  data.hp = hp;
  data.maxHp = maxHp;
  data.mana = mana;
  data.maxMana = maxMana;
  data.experience = experience;
  data.gold = gold;
  data.strength = strength;
  data.agility = agility;
  data.constitution = constitution;
  data.intelligence = intelligence;
  data.inventory = inventory.getItems();
  data.equippedWeapon = inventory.getWeapon();
  data.equippedArmor = inventory.getArmor();
  data.equippedHelmet = inventory.getHelmet();
  data.equippedShield = inventory.getShield();
  return data;
}

void PlayerInfo::fromPlayerData(const PlayerData &data) {
  name = data.name;
  password = data.password;
  race = RaceUtils::stringToRace(data.race);
  playerClass = data.playerClass;
  x = data.x;
  y = data.y;
  direction = static_cast<Direction>(data.direction);
  level = data.level;
  hp = data.hp;
  maxHp = data.maxHp;
  mana = data.mana;
  maxMana = data.maxMana;
  experience = data.experience;
  gold = data.gold;
  strength = data.strength;
  agility = data.agility;
  constitution = data.constitution;
  intelligence = data.intelligence;
  inventory.setItems(data.inventory);
  inventory.setWeapon(data.equippedWeapon);
  inventory.setArmor(data.equippedArmor);
  inventory.setHelmet(data.equippedHelmet);
  inventory.setShield(data.equippedShield);
}

static std::string lowercase(const std::string &s) {
  std::string result = s;
  auto toLower = [](unsigned char c) { return std::tolower(c); };
  std::transform(result.begin(), result.end(), result.begin(), toLower);
  return result;
}

static void initPlayerStats(PlayerInfo &player, const Race race,
                            const std::string &playerClass) {
  struct BaseStats {
    uint32_t strength, agility, constitution, intelligence;
  };

  auto getRaceStats = [](const Race &race) -> BaseStats {
    switch (race) {
    case Race::Human:
      return {10, 10, 10, 10};
    case Race::Elf:
      return {6, 13, 5, 16};
    case Race::Dwarf:
      return {13, 4, 16, 7};
    case Race::Gnome:
      return {7, 6, 14, 13};
    }
    throw std::invalid_argument("Invalid race");
  };

  auto getClassStats = [](const std::string &c) -> BaseStats {
    std::string lc = lowercase(c);
    if (lc == "mago")
      return {3, 5, 5, 15};
    if (lc == "clerigo")
      return {7, 7, 9, 10};
    if (lc == "paladin")
      return {10, 6, 10, 8};
    return {10, 8, 10, 3};
  };

  auto raceStats = getRaceStats(race);
  auto classStats = getClassStats(playerClass);
  player.strength = raceStats.strength + classStats.strength;
  player.agility = raceStats.agility + classStats.agility;
  player.constitution = raceStats.constitution + classStats.constitution;
  player.intelligence = raceStats.intelligence + classStats.intelligence;
}

bool PlayerInfo::colisionaCon(int targetX, int targetY, int ancho,
                              int alto) const {
  return !(targetX + ancho <= x || targetX >= x + ANCHO ||
           targetY + alto <= y || targetY >= y + ALTO);
}

int PlayerInfo::getX() const { return x; }

int PlayerInfo::getY() const { return y; }

int PlayerInfo::getAncho() const { return ANCHO; }

int PlayerInfo::getAlto() const { return ALTO; }

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
                          const std::string &playerClass,
                          uint32_t connectionId) {

  if (repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId,
                                    RegisterPlayerEventDTO{0, 1, race});
    return;
  }

  uint32_t newId = nextPlayerId++;
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 64;

  auto player =
      std::make_unique<PlayerInfo>(newId, spawnX, spawnY, Direction::Down);
  player->name = name;
  player->race = race;
  player->playerClass = playerClass;

  initPlayerStats(*player, race, playerClass);

  player->maxHp = Formulas::calcularVidaMax(player->constitution, race,
                                            playerClass, player->level);
  player->hp = player->maxHp;
  player->maxMana = Formulas::calcularManaMax(player->intelligence, race,
                                              playerClass, player->level);
  player->mana = player->maxMana;

  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(connectionId,
                                  RegisterPlayerEventDTO{newId, 0, race});

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
         info->direction, info->race, info->name, info->hp, info->maxHp,
         info->mana, info->maxMana, info->gold, info->level, info->experience});
  }

  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(PlayerAppearedEventDTO{
      newId, static_cast<int16_t>(spawnX), static_cast<int16_t>(spawnY),
      Direction::Down, race, name, players[newId]->hp, players[newId]->maxHp,
      players[newId]->mana, players[newId]->maxMana, players[newId]->gold,
      players[newId]->level, players[newId]->experience});
}

void Game::loginPlayer(const std::string &name, uint32_t connectionId) {

  if (!repository.exists(name)) {
    senderQueueMonitor.sendToClient(connectionId,
                                    RegisterPlayerEventDTO{0, 1, Race::Human});
    return;
  }

  for (auto &[pid, info] : players) {
    if (info->name == name) {
      senderQueueMonitor.sendToClient(connectionId,
                                      RegisterPlayerEventDTO{0, 2, info->race});
      return;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = nextPlayerId++;

  auto player = std::make_unique<PlayerInfo>(
      newId, data.x, data.y, static_cast<Direction>(data.direction));
  player->fromPlayerData(data);

  colisionables.push_back(player.get());
  players[newId] = std::move(player);
  connectionToPlayer[connectionId] = newId;
  playerToConnection[newId] = connectionId;

  senderQueueMonitor.markAsRegistered(connectionId);

  senderQueueMonitor.sendToClient(
      connectionId, RegisterPlayerEventDTO{newId, 0, players[newId]->race});

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
         info->direction, info->race, info->name, info->hp, info->maxHp,
         info->mana, info->maxMana, info->gold, info->level, info->experience});
  }
  senderQueueMonitor.sendToClient(connectionId,
                                  PlayerListEventDTO{std::move(playerList)});

  messagesToSend.push_back(PlayerAppearedEventDTO{
      newId, static_cast<int16_t>(data.x), static_cast<int16_t>(data.y),
      static_cast<Direction>(data.direction), players[newId]->race,
      players[newId]->name, players[newId]->hp, players[newId]->maxHp,
      players[newId]->mana, players[newId]->maxMana, players[newId]->gold,
      players[newId]->level, players[newId]->experience});
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end()) {
    return;
  }

  PlayerInfo &player = *it->second;
  player.direction = direction;
  player.moving = true;

  int targetX = player.x;
  int targetY = player.y;
  switch (direction) {
  case Direction::Up:
    targetY -= 1;
    break;
  case Direction::Down:
    targetY += 1;
    break;
  case Direction::Left:
    targetX -= 1;
    break;
  case Direction::Right:
    targetX += 1;
    break;
  }

  for (auto &col : colisionables) {
    if (col == it->second.get())
      continue;
    if (col->colisionaCon(targetX, targetY, player.getAncho(),
                          player.getAlto())) {
      return;
    }
  }

  {
    int start_i = floorDiv(targetX, gridSize) + maxSize / 2;
    int end_i =
        floorDiv(targetX + player.getAncho() - 1, gridSize) + maxSize / 2;
    int start_j = floorDiv(targetY, gridSize) + maxSize / 2;
    int end_j =
        floorDiv(targetY + player.getAlto() - 1, gridSize) + maxSize / 2;
    for (int i = start_i; i <= end_i; i++) {
      for (int j = start_j; j <= end_j; j++) {
        if (collidableCells.count({i, j, 0})) {
          return;
        }
      }
    }
  }

  player.x = targetX;
  player.y = targetY;

  messagesToSend.push_back(
      PlayerMovedEventDTO{playerId, static_cast<int16_t>(player.x),
                          static_cast<int16_t>(player.y), direction});
}

void Game::stopPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }
  PlayerInfo &player = *it->second;
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
  if (it == players.end())
    return;
  it->second->inventory.removeItem(inventorySlot);
}

void Game::movePlayers() {
  for (auto &[playerID, info] : players) {
    if (!info->moving)
      continue;

    int targetX = info->x;
    int targetY = info->y;
    switch (info->direction) {
    case Direction::Up:
      targetY -= 1;
      break;
    case Direction::Down:
      targetY += 1;
      break;
    case Direction::Left:
      targetX -= 1;
      break;
    case Direction::Right:
      targetX += 1;
      break;
    }

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
