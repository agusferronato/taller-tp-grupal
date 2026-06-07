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
#include <optional>
#include <set>
#include <tuple>

#include "Biome.h"
#include "Character.h"
#include "City.h"
#include "Colisionable.h"
#include "Direction.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Inventory.h"
#include "InventoryManager.h"
#include "ItemDef.h"
#include "MapData.h"
#include "NPC.h"
#include "PlayerData.h"
#include "PlayerRepository.h"
#include "Queue.h"
#include "Race.h"
#include "SenderQueueMonitor.h"
#include "Thread.h"

class Game : public Thread {

private:
  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  PlayerRepository &repository;

  std::list<ServerEventDTO> messagesToSend;
  bool keepRunning = true;

  std::unordered_map<std::string, uint32_t> playerIdByName;

  uint32_t nextPlayerId{1};
  int nextSpawnX{0};
  std::unordered_map<uint32_t, std::unique_ptr<Character>> players;
  std::vector<Colisionable *> colisionables;
  std::unordered_map<uint32_t, uint32_t> connectionToPlayer;
  std::unordered_map<uint32_t, uint32_t> playerToConnection;

  InventoryManager inventoryManager{players, messagesToSend};

  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::list<TileOrigin> textureOrigins;
  std::set<std::tuple<int, int, int>> collidableCells;

  std::list<std::unique_ptr<Biome>> biomes;
  std::list<City> cities;
  std::list<std::unique_ptr<NPC>> npcs;

public:
  Game(Queue<ClientMessage> &gameloopQueue,
       SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository);

  virtual void run() override;

  void kill();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void registerPlayer(const std::string &name, const Race race,
                      const PlayerClass playerClass, uint32_t connectionId);
  void loginPlayer(const std::string &name, uint32_t connectionId);
  void movePlayer(uint32_t playerId, Direction direction);
  void stopPlayer(uint32_t playerId);
  void exitPlayer(uint32_t playerId);
  void exitPlayerByConnection(uint32_t connectionId);
  void equipItem(uint32_t playerId, uint8_t inventorySlot);
  void unequipSlot(uint32_t playerId, uint8_t equipSlot);
  void dropItem(uint32_t playerId, uint8_t inventorySlot);
  void takeItem(uint32_t playerId);
  void sendGlobalChatMessage(uint32_t playerId, const std::string &message);
  void atack(uint32_t playerId, int16_t x, int16_t y);

  bool thereIsACollidableEntityAt(Position position);
  void appearNPC(std::unique_ptr<NPC> &&npc);
  uint16_t nextNPCId{1};

private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
  void saveAllPlayers();

  void appearNPCs();

  void playerAtackPlayer(Character &atacker, Character &target);
  void playerAtackNPC(Character &atacker, NPC &target);
  uint32_t calculateDamage(Character &atacker);
  bool validAtack(Character &atacker, Character &target);
  Character *findPlayerByCoordinates(int16_t x, int16_t y);
  NPC *findNPCByCoordinates(int16_t x, int16_t y);
  const std::string npcName(NPC &npc);
};

#endif
