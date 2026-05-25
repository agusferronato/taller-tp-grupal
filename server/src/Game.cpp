#include <algorithm>

#include "Game.h"
#include "DTO/Events/PlayerAppearedEventDTO.h"
#include "DTO/Events/PlayerListDTO.h"
#include "DTO/Events/PlayerMovedEventDTO.h"
#include "DTO/Events/PlayerRemovedEventDTO.h"
#include "DTO/Events/PlayerStoppedDTO.h"
#include "DTO/Events/RegisterPlayerResponseDTO.h"
#include "command/CommandFactory.h"

PlayerInfo::PlayerInfo(uint32_t id, int x, int y, Direction dir)
    : id(id), x(x), y(y), direction(dir) {}

PlayerData PlayerInfo::toPlayerData() const {
  PlayerData data{};
  data.setName(name);
  data.setPassword(password);
  data.setRace(race);
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
  return data;
}

void PlayerInfo::fromPlayerData(const PlayerData &data) {
  name = data.name;
  password = data.password;
  race = data.race;
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
}

bool PlayerInfo::colisionaCon(int targetX, int targetY, int ancho, int alto) const {
  return !(targetX + ancho <= x || targetX >= x + ANCHO ||
           targetY + alto <= y || targetY >= y + ALTO);
}

int PlayerInfo::getX() const { return x; }

int PlayerInfo::getY() const { return y; }

int PlayerInfo::getAncho() const { return ANCHO; }

int PlayerInfo::getAlto() const { return ALTO; }

Game::Game(Queue<ClientRequestDTO> &gameloopQueue,
           SenderQueueMonitor &senderQueueMonitor,
           PlayerRepository &repository)
    : gameloopQueue(gameloopQueue), senderQueueMonitor(senderQueueMonitor),
      repository(repository) {}

void Game::run() {

  ConstantRateLoop rateloop(FPS_SERVER);
  CommandFactory factory;
  unsigned int it = 0;
  unsigned int saveCounter = 0;

  while (keepRunning) {

    auto dto = gameloopQueue.pop();
    auto command = factory.create(std::move(dto));

    command->execute(*this);

    sendMessages();

    if (++saveCounter >= 300) {
      for (auto &[id, player] : players) {
        repository.save(player->name, player->toPlayerData());
      }
      saveCounter = 0;
    }

    rateloop.updateTimer(it);
  }

  saveAllPlayers();
}

void Game::kill() { keepRunning = false; }

void Game::registerPlayer(const std::string &name) {

  if (repository.exists(name)) {
    messagesToSend.push_back(RegisterPlayerResponseDTO{0, 1});
    return;
  }

  uint32_t newId = nextPlayerId++;
  int spawnX = nextSpawnX;
  int spawnY = 0;
  nextSpawnX += 64;

  auto player = std::make_unique<PlayerInfo>(newId, spawnX, spawnY, Direction::Down);
  player->name = name;

  repository.create(player->toPlayerData());

  colisionables.push_back(player.get());
  players[newId] = std::move(player);

  messagesToSend.push_back(RegisterPlayerResponseDTO{newId, 0});

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back({pid, static_cast<int16_t>(info->x),
                          static_cast<int16_t>(info->y), info->direction});
  }
  messagesToSend.push_back(PlayerListDTO{std::move(playerList)});

  messagesToSend.push_back(
      PlayerAppearedEventDTO{newId, static_cast<int16_t>(spawnX), static_cast<int16_t>(spawnY), Direction::Down});
}

void Game::loginPlayer(const std::string &name) {

  if (!repository.exists(name)) {
    messagesToSend.push_back(RegisterPlayerResponseDTO{0, 1});
    return;
  }

  for (auto it = players.begin(); it != players.end(); ++it) {
    if (it->second->name == name) {
      colisionables.erase(std::remove(colisionables.begin(),
                                      colisionables.end(), it->second.get()),
                          colisionables.end());
      players.erase(it);
      break;
    }
  }

  PlayerData data = repository.load(name);
  uint32_t newId = nextPlayerId++;

  auto player = std::make_unique<PlayerInfo>(newId, data.x, data.y,
                                              static_cast<Direction>(data.direction));
  player->fromPlayerData(data);

  colisionables.push_back(player.get());
  players[newId] = std::move(player);

  messagesToSend.push_back(RegisterPlayerResponseDTO{newId, 0});

  std::vector<PlayerInfoDTO> playerList;
  for (auto &[pid, info] : players) {
    playerList.push_back({pid, static_cast<int16_t>(info->x),
                          static_cast<int16_t>(info->y), info->direction});
  }
  messagesToSend.push_back(PlayerListDTO{std::move(playerList)});

  messagesToSend.push_back(
      PlayerAppearedEventDTO{newId, static_cast<int16_t>(data.x),
                              static_cast<int16_t>(data.y),
                              static_cast<Direction>(data.direction)});
}

void Game::movePlayer(uint32_t playerId, Direction direction) {

  auto it = players.find(playerId);

  if (it == players.end()) {
    return;
  }

  PlayerInfo &player = *it->second;
  player.direction = direction;

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
    if (col->colisionaCon(targetX, targetY, player.getAncho(), player.getAlto())) {
      return;
    }
  }

  player.x = targetX;
  player.y = targetY;

  std::cout << "player: " << playerId << " moved to x: " << player.x
            << " y: " << player.y << std::endl;

  messagesToSend.push_back(
      PlayerMovedEventDTO{playerId, static_cast<int16_t>(player.x),
                          static_cast<int16_t>(player.y), direction});
}

void Game::stopPlayer(uint32_t playerId) {
  messagesToSend.push_back(PlayerStoppedDTO{playerId});
}

void Game::exitPlayer(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end()) {
    return;
  }

  repository.save(it->second->name, it->second->toPlayerData());

  colisionables.erase(std::remove(colisionables.begin(), colisionables.end(),
                                   it->second.get()),
                      colisionables.end());

  messagesToSend.push_back(PlayerRemovedEventDTO{playerId});

  players.erase(it);
}

void Game::saveAllPlayers() {
  for (auto &[id, player] : players) {
    repository.save(player->name, player->toPlayerData());
  }
}

void Game::execute(ClientRequestDTO clientMessage) {
  CommandFactory factory;
  auto command = factory.create(std::move(clientMessage));
  command->execute(*this);
}

void Game::sendMessages() {
  if (messagesToSend.empty())
    return;
  senderQueueMonitor.broadCast(messagesToSend);
  messagesToSend.clear();
}
