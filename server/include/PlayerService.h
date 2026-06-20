#ifndef PLAYER_SERVICE_H
#define PLAYER_SERVICE_H

#include "Character.h"
#include "City.h"
#include "ClanManager.h"
#include "Colisionable.h"
#include "InventoryManager.h"
#include "NPC.h"
#include "PlayerRepository.h"
#include "SenderQueueMonitor.h"
#include <cstdint>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class PlayerService {
private:
  SenderQueueMonitor &senderQueueMonitor;
  PlayerRepository &repository;
  ClanManager &clanManager;
  std::list<ServerEventDTO> &messagesToSend;
  std::vector<Colisionable *> &colisionables;
  std::unordered_map<uint32_t, std::unique_ptr<Character>> players;
  std::unordered_map<std::string, uint32_t> playerIdByName;

  int nextSpawnX{0};

private:
  const int maxGridSize;
  const int gridSize;
  const int commonGroundTextureId;
  const std::list<City> &cities;
  const std::list<std::unique_ptr<NPC>> &npcs;
  const std::list<TileOrigin> &textureOrigins;
  const std::vector<GroundItem> &groundItems;

public:
  PlayerService(SenderQueueMonitor &senderQueueMonitor,
                PlayerRepository &repository, ClanManager &clanManager,
                std::list<ServerEventDTO> &messagesToSend,
                std::vector<Colisionable *> &colisionables,
                std::list<City> &cities, std::list<std::unique_ptr<NPC>> &npcs,
                const int maxGridSize, const int gridSize,
                const int commonGroundTextureId,
                const std::list<TileOrigin> &textureOrigins,
                const std::vector<GroundItem> &groundItems);

  std::unordered_map<uint32_t, std::unique_ptr<Character>> &getPlayers();
  std::optional<Character *> getPlayer(uint32_t playerID);
  std::optional<Character *> getPlayer(const std::string &name);

  void registerPlayer(const std::string &name, const Race race,
                      const PlayerClass playerClass, uint32_t connectionId);
  void validateLogin(const std::string &name, uint32_t connectionId);
  void loginPlayer(const std::string &name, uint32_t connectionId);
  void exitPlayer(uint32_t playerId);

  std::vector<uint32_t> getClanOfPlayerId(const uint32_t playerId);
  std::vector<uint32_t> getClanOfPlayer(const Character &player);

private:
  void sendPlayerNewConnection(Character &player);
  std::pair<int, int> playerSpawnPosition();
};

#endif
