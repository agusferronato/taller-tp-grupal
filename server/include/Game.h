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

#include "Banker.h"
#include "Biome.h"
#include "BiomeData.h"
#include "Character.h"
#include "CheatType.h"
#include "City.h"
#include "CityEntitiesStoreData.h"
#include "CityEntityCommandDTO.h"
#include "ClanManager.h"
#include "Colisionable.h"
#include "CombatSystem.h"
#include "DTO/Commands/ClientCommandDTO.h"
#include "DTO/Events/EventDTO.h"
#include "Direction.h"
#include "Inventory.h"
#include "InventoryManager.h"
#include "MapData.h"
#include "MapLoader.h"
#include "NPC.h"
#include "NPCData.h"
#include "PlayerCheats.h"
#include "PlayerData.h"
#include "PlayerRepository.h"
#include "PlayerService.h"
#include "Priest.h"
#include "Queue.h"
#include "Race.h"
#include "SenderQueueMonitor.h"
#include "Thread.h"
#include "Trader.h"

class Game : public Thread {

private:
  Queue<ClientMessage> &gameloopQueue;
  SenderQueueMonitor &senderQueueMonitor;
  PlayerRepository &repository;
  ClanManager &clanManager;
  std::list<ServerEventDTO> messagesToSend;
  bool keepRunning = true;

  int nextSpawnX{0};
  std::vector<Colisionable *> colisionables;
  std::unordered_map<uint32_t, PlayerCheats> cheatsByPlayer;

  MapLoader mapLoader;
  int maxSize;
  int gridSize;
  int commonGroundTextureId;
  std::list<TileOrigin> textureOrigins;
  std::set<std::tuple<int, int, int>> collidableCells;

  std::list<std::unique_ptr<Biome>> biomes;
  std::list<City> cities;
  std::list<std::unique_ptr<NPC>> npcs;

  CityEntitiesStoreData storeData;

  std::vector<GroundItem> groundItems;
  PlayerService playerService;
  std::unordered_map<uint32_t, std::unique_ptr<Character>> &players;
  InventoryManager inventoryManager{players, messagesToSend, groundItems};

  CombatSystem combatSystem;

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
  void equipItem(uint32_t playerId, uint8_t inventorySlot);
  void unequipSlot(uint32_t playerId, uint8_t equipSlot);
  void dropItem(uint32_t playerId, uint8_t inventorySlot);
  void takeItem(uint32_t playerId);
  void sendGlobalChatMessage(uint32_t playerId, const std::string &message);
  void sendPrivateMessage(uint32_t connectionId, const std::string &targetName,
                          const std::string &message);
  void attack(uint32_t playerId, int16_t x, int16_t y);
  void applyCheat(uint32_t connectionId, CheatType cheat, uint32_t arg = 0,
                  const std::string &itemName = "");

  void createClan(uint32_t playerId, const std::string &clanName);
  void requestJoinClan(uint32_t playerId, const std::string &clanName);
  void acceptClanRequest(uint32_t founderId, const std::string &playerName);
  void rejectClanRequest(uint32_t founderId, const std::string &playerName);
  void banClanPlayer(uint32_t founderId, const std::string &playerName);
  void kickClanMember(uint32_t founderId, const std::string &playerName);
  void leaveClan(uint32_t playerId);
  void reviewClan(uint32_t playerId);

  void startMeditating(uint32_t playerId);
  void stopMeditating(uint32_t playerId);

  void executeCityEntityCommand(uint32_t playerId, uint8_t type,
                                const std::string &arg);
  void sendInventoryUpdate(uint32_t playerId);
  void sendPlayerInfoUpdate(uint32_t playerId);
  void sendPlayerMoved(uint32_t playerId);

  bool thereIsACollidableEntityAt(Position position);
  uint32_t appearNPC(std::unique_ptr<NPC> &&npc);
  uint32_t nextNPCId{1};
  uint32_t nextCityEntityId{1};

  struct ResurrectingPlayer {
    Character *character;
    int priestX;
    int priestY;
    int counter;
    int maxCounter;
  };

  void addResurrectingPlayer(Character &character, int priestX, int priestY,
                             int maxCounter);

  void sendSystemMessage(uint32_t playerId, const std::string &msg);
  void sendSystemMessageToPlayer(uint32_t playerId, const std::string &message);

  void sendExistingPlayersInventory(uint32_t connectionId,
                                    uint32_t newPlayerId);

private:
  void execute(ClientMessage clientMessage);
  void sendMessages();
  void movePlayers();
  void saveAllPlayers();

  void appearNPCs();

  std::string getPlayerName(uint32_t playerId);
  void sendToPlayer(uint32_t playerId, const ServerEventDTO &event);
  void sendToPlayers(const std::vector<uint32_t> &playerIds,
                     const ServerEventDTO &event);
  void sendToClan(uint32_t clanId, const ServerEventDTO &event,
                  std::optional<uint32_t> exceptPlayerId = std::nullopt);

  void sendErrorMessageToConnection(uint32_t connectionId,
                                    const std::string &message);
  uint32_t movementSpeedFor(uint32_t playerId) const;
  bool hasInfiniteHealth(uint32_t playerId) const;
  bool hasInfiniteMana(uint32_t playerId) const;

  void makeNPCsfollowPlayers();
  void makeCitiesEntitiesFollowPlayers();
  void createCityEntities();

  bool checkIfItCollides(Colisionable *entity);
  int floorDiv(int a, int b);

  void restorePlayers();

  bool isNearEntity(CityEntity &entity, const Character &character);
  CityEntity *findNearestEntity(uint32_t playerId, CityEntityType type);

  void updateResurrectingPlayers();
  bool isResurrecting(uint32_t playerId);
  bool consumeManaForAttack(Character &attacker);

  std::list<ResurrectingPlayer> resurrectingPlayers;
};

#endif
