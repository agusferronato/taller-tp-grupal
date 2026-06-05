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
#include <optional>

#include "ClientMessage.h"
#include <set>
#include <tuple>

#include "Biome.h"
#include "Character.h"
#include "City.h"
#include "Colisionable.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "Inventory.h"
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

  
  uint32_t nextPlayerId{1};
  int nextSpawnX{0};
  std::unordered_map<uint32_t, std::unique_ptr<Character>> players;
  std::unordered_map<std::string, uint32_t> playerIdByName;
  std::vector<Colisionable *> colisionables;
  std::unordered_map<uint32_t, uint32_t> connectionToPlayer;
  std::unordered_map<uint32_t, uint32_t> playerToConnection;

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
  void sendGlobalChatMessage(uint32_t playerId, const std::string &message);

  bool thereIsACollidableEntityAt(Position position);
  void appearNPC(std::unique_ptr<NPC> &&npc);
  uint16_t nextNPCId{1};

private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
  void saveAllPlayers();

  void appearNPCs();

  std::optional<uint32_t> findPlayerIdByName(const std::string& name) const;
  std::string getPlayerName(uint32_t playerId) const;
};

#endif
