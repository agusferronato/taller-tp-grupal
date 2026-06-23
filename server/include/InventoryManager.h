#ifndef INVENTORY_MANAGER_H
#define INVENTORY_MANAGER_H

#include <cstdint>
#include <list>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Character.h"
#include "DTO/Events/EventDTO.h"

struct GroundItem {
  uint32_t id;
  uint8_t itemId;
  int x;
  int y;
};

class InventoryManager {
public:
  InventoryManager(
      std::unordered_map<uint32_t, std::unique_ptr<Character>> &players,
      std::list<ServerEventDTO> &messagesToSend,
      std::vector<GroundItem> &groundItems);

  void equipItem(uint32_t playerId, uint8_t slotIndex);
  void unequipSlot(uint32_t playerId, EquipSlot slot);
  void dropItem(uint32_t playerId, uint8_t slotIndex);
  void takeItem(uint32_t playerId);
  void addGroundItem(uint8_t itemId, int x, int y);

private:
  std::unordered_map<uint32_t, std::unique_ptr<Character>> &players;
  std::list<ServerEventDTO> &messagesToSend;
  std::vector<GroundItem> &groundItems;
  uint32_t nextGroundItemId{1};

  void broadcastInventoryUpdate(Character &player);
  void broadcastPlayerInfo(Character &player);
  void consumePotion(Character &player, uint8_t itemId);
  bool isOnGroundItem(int px, int py, const GroundItem &gi) const;
};

#endif
