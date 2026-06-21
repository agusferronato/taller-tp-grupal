#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "Biome.h"
#include "Character.h"
#include "City.h"
#include "ClanManager.h"
#include "Colisionable.h"
#include "DTO/Events/EventDTO.h"
#include "InventoryManager.h"
#include "NPC.h"
#include "PlayerCheats.h"
#include "PlayerService.h"
#include "SenderQueueMonitor.h"
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

class CombatSystem {
private:
  PlayerService &playerService;
  std::list<std::unique_ptr<NPC>> &npcs;
  const std::list<City> &cities;
  std::vector<Colisionable *> &colisionables;
  const std::list<std::unique_ptr<Biome>> &biomes;
  std::list<ServerEventDTO> &messagesToSend;
  SenderQueueMonitor &senderQueueMonitor;
  InventoryManager &inventoryManager;
  const ClanManager &clanManager;
  std::unordered_map<uint32_t, PlayerCheats>
      &cheatsByPlayer; // los cheats no debria etsar dentro de Character.cpp?
  const int gridSize;
  const int maxSize;

public:
  CombatSystem(PlayerService &playerService,
               std::list<std::unique_ptr<NPC>> &npcs,
               const std::list<City> &cities,
               std::vector<Colisionable *> &colisionables,
               const std::list<std::unique_ptr<Biome>> &biomes,
               std::list<ServerEventDTO> &messagesToSend,
               SenderQueueMonitor &senderQueueMonitor,
               InventoryManager &inventoryManager,
               const ClanManager &clanManager,
               std::unordered_map<uint32_t, PlayerCheats> &cheatsByPlayer,
               const int gridSize, const int maxSize);

  void attack(Character &attacker, int16_t x, int16_t y);
  void tryAttack(NPC &npc, Character &target);
  void killPlayer(Character &dyingPlayer);
  bool hasInfiniteHealth(uint32_t playerId) const;

private:
  void playerAttackPlayer(Character &attacker, Character &target);
  void playerAttackNPC(Character &attacker, NPC &target);
  bool validAttack(Character &attacker, Character &target);
  bool validAttackToNpc(Character &attacker);
  Character *findPlayerByCoordinates(int16_t x, int16_t y);
  NPC *findNPCByCoordinates(int16_t x, int16_t y);
  uint32_t calculateDamage(Character &attacker);
  bool consumeManaForAttack(Character &attacker);
};

#endif
