#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ClientMessage.h"
#include <set>
#include <tuple>

#include "Biome.h"
#include "Character.h"
#include "CheatType.h"
#include "City.h"
#include "ClanManager.h"
#include "Colisionable.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
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
  struct PlayerCheats {
    bool infiniteHealth{false};
    bool infiniteMana{false};
    bool superSpeed{false};
  };

  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  PlayerRepository &repository;
  ClanManager &clanManager;

  std::list<ServerEventDTO> messagesToSend;
  bool keepRunning = true;

  uint32_t nextPlayerId{1};
  int nextSpawnX{0};
  std::unordered_map<uint32_t, std::unique_ptr<Character>> players;
  std::unordered_map<std::string, uint32_t> playerIdByName;
  std::vector<Colisionable *> colisionables;
  std::unordered_map<uint32_t, uint32_t> connectionToPlayer;
  std::unordered_map<uint32_t, uint32_t> playerToConnection;
  std::unordered_map<uint32_t, PlayerCheats> cheatsByPlayer;

  InventoryManager inventoryManager{players, messagesToSend};

  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::list<TileOrigin> textureOrigins;
  std::set<std::tuple<int, int, int>> collidableCells;

  std::list<std::unique_ptr<Biome>> biomes;
  std::list<City> cities;
  std::list<std::unique_ptr<NPC>> npcs;

  std::string mapPath;

public:
  Game(Queue<ClientMessage> &gameloopQueue,
       SenderQueueMonitor &senderQueueMonitor, PlayerRepository &repository,
       ClanManager &clanManager, const std::string &mapPath);

  virtual void run() override;

  void kill();

  Game(const Game &) = delete;
  Game &operator=(const Game &) = delete;

  void registerPlayer(const std::string &name, const Race race,
                      const PlayerClass playerClass, uint32_t connectionId);
  void validateLogin(const std::string &name, uint32_t connectionId);
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
  void sendPrivateMessage(uint32_t connectionId, const std::string &targetName,
                          const std::string &message);
  void attack(uint32_t playerId, int16_t x, int16_t y);
  void applyCheat(uint32_t connectionId, CheatType cheat);

  void createClan(uint32_t playerId, const std::string &clanName);
  void requestJoinClan(uint32_t playerId, const std::string &clanName);
  void acceptClanRequest(uint32_t founderId, const std::string &playerName);
  void rejectClanRequest(uint32_t founderId, const std::string &playerName);
  void banClanPlayer(uint32_t founderId, const std::string &playerName);
  void kickClanMember(uint32_t founderId, const std::string &playerName);
  void leaveClan(uint32_t playerId);
  void reviewClan(uint32_t playerId);

  bool thereIsACollidableEntityAt(Position position);
  void appearNPC(std::unique_ptr<NPC> &&npc);
  uint32_t nextNPCId{1};
  uint32_t nextCityEntityId{1};

private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
  void saveAllPlayers();

  void appearNPCs();

  std::optional<uint32_t> findPlayerIdByName(const std::string &name) const;
  std::string getPlayerName(uint32_t playerId) const;
  std::optional<uint32_t> getConnectionIdForPlayer(uint32_t playerId) const;
  void sendToPlayer(uint32_t playerId, const ServerEventDTO &event);
  void sendToPlayers(const std::vector<uint32_t> &playerIds,
                     const ServerEventDTO &event);
  void sendToClan(uint32_t clanId, const ServerEventDTO &event,
                  std::optional<uint32_t> exceptPlayerId = std::nullopt);
  void sendSystemMessageToPlayer(uint32_t playerId, const std::string &message);
  void sendErrorMessageToConnection(uint32_t connectionId,
                                    const std::string &message);
  uint32_t movementSpeedFor(uint32_t playerId) const;
  bool hasInfiniteHealth(uint32_t playerId) const;
  bool hasInfiniteMana(uint32_t playerId) const;

  void makeNPCsfollowPlayers();
  void makeCitiesEntitiesFollowPlayers();
  void createCityEntities();

  bool checkIfItCollides(Colisionable *entity);

  void playerAttackPlayer(Character &attacker, Character &target);
  void playerAttackNPC(Character &attacker, NPC &target);
  uint32_t calculateDamage(Character &attacker);
  bool validAttack(Character &attacker, Character &target);
  Character *findPlayerByCoordinates(int16_t x, int16_t y);
  NPC *findNPCByCoordinates(int16_t x, int16_t y);
  int floorDiv(int a, int b) { return (a >= 0) ? a / b : (a - b + 1) / b; }

  void killPlayer(Character &dyingPlayer);
};

#endif
