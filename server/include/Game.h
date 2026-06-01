#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ClientMessage.h"
#include <set>
#include <tuple>

#include "Colisionable.h"
#include "Direction.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "Inventory.h"
#include "ItemDef.h"
#include "MapData.h"
#include "PlayerData.h"
#include "PlayerRepository.h"
#include "Queue.h"
#include "SenderQueueMonitor.h"
#include "Thread.h"
#include "NPC.h"
#include <Biome.h>

class PlayerInfo : public Colisionable {
public:
  static constexpr int ANCHO = 32;
  static constexpr int ALTO = 32;

  uint32_t id;
  int x{0}, y{0};
  Direction direction{Direction::Down};
  bool moving{false};

  std::string name;
  std::string password;
  std::string race;
  std::string playerClass;
  uint32_t level{1};
  uint32_t hp{100}, maxHp{100};
  uint32_t mana{0}, maxMana{0};
  uint32_t experience{0};
  uint32_t gold{0};
  uint32_t strength{};
  uint32_t agility{};
  uint32_t constitution{};
  uint32_t intelligence{};
  Inventory inventory;

  PlayerInfo(uint32_t id, int x, int y, Direction dir);

  PlayerData toPlayerData() const;
  void fromPlayerData(const PlayerData &data);

  bool colisionaCon(int targetX, int targetY, int ancho, int alto) const override;
  int getX() const override;
  int getY() const override;
  int getAncho() const override;
  int getAlto() const override;
};

class Game : public Thread {

private:
  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  PlayerRepository &repository;

  std::list<ServerEventDTO> messagesToSend;
  bool keepRunning = true;
  uint32_t nextPlayerId{1};
  int nextSpawnX{0};
  std::unordered_map<uint32_t, std::unique_ptr<PlayerInfo>> players;
  std::vector<Colisionable*> colisionables;
  std::unordered_map<uint32_t, uint32_t> connectionToPlayer;
  std::unordered_map<uint32_t, uint32_t> playerToConnection;

  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::list<TileOrigin> textureOrigins;
  std::set<std::tuple<int, int, int>> collidableCells;

  std::list<std::unique_ptr<Biome>> biomes;


public:
  Game(Queue<ClientMessage> &gameloopQueue,
       SenderQueueMonitor &senderQueueMonitor,
       PlayerRepository &repository);

  virtual void run() override;

  void kill();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void registerPlayer(const std::string &name, const std::string &race, const std::string &playerClass, uint32_t connectionId);
  void loginPlayer(const std::string &name, uint32_t connectionId);
  void movePlayer(uint32_t playerId, Direction direction);
  void stopPlayer(uint32_t playerId);
  void exitPlayer(uint32_t playerId);
  void exitPlayerByConnection(uint32_t connectionId);
  void equipItem(uint32_t playerId, uint8_t inventorySlot);
  void unequipSlot(uint32_t playerId, uint8_t equipSlot);
  void dropItem(uint32_t playerId, uint8_t inventorySlot);


  bool thereIsACollidableEntityAt(Position position);
  void appearNPC(std::unique_ptr<NPC>&& npc);


private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
  void saveAllPlayers();

  void appearNPCs();
};

#endif