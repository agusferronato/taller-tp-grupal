#include "InventoryManager.h"

#include "DTO/Events/GroundItemAppearedEventDTO.h"
#include "DTO/Events/GroundItemRemovedEventDTO.h"
#include "DTO/Events/InventoryUpdateEventDTO.h"
#include "DTO/Events/PlayerInfoEventDTO.h"
#include "InventoryConstants.h"

InventoryManager::InventoryManager(
    std::unordered_map<uint32_t, std::unique_ptr<Character>> &players,
    std::list<ServerEventDTO> &messagesToSend)
    : players(players), messagesToSend(messagesToSend) {}

void InventoryManager::equipItem(uint32_t playerId, uint8_t slotIndex) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;

  Character &player = *it->second;
  uint8_t itemId = player.getInventory().getItemId(slotIndex);
  if (itemId == EMPTY_ITEM)
    return;

  const ItemDef &def = ITEM_TABLE[itemId];

  if (def.type == ItemType::PotionHp || def.type == ItemType::PotionMana) {
    if (!player.removeItem(slotIndex))
      return;
    consumePotion(player, def);
    broadcastInventoryUpdate(player);
    broadcastPlayerInfo(player);
    return;
  }

  if (!player.equipItem(slotIndex))
    return;

  broadcastInventoryUpdate(player);
}

void InventoryManager::unequipSlot(uint32_t playerId, EquipSlot slot) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;

  Character &player = *it->second;
  if (!player.unequipSlot(slot))
    return;

  broadcastInventoryUpdate(player);
}

void InventoryManager::dropItem(uint32_t playerId, uint8_t slotIndex) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;

  Character &player = *it->second;
  uint8_t itemId = player.getInventory().getItemId(slotIndex);
  if (itemId == EMPTY_ITEM)
    return;

  const ItemDef &def = ITEM_TABLE[itemId];

  if (def.type == ItemType::PotionHp || def.type == ItemType::PotionMana) {
    if (!player.removeItem(slotIndex))
      return;
    broadcastInventoryUpdate(player);
    return;
  }

  if (!player.removeItem(slotIndex))
    return;

  GroundItem gi;
  gi.id = nextGroundItemId++;
  gi.itemId = itemId;
  gi.x = player.getX();
  gi.y = player.getY();
  groundItems.push_back(gi);

  messagesToSend.push_back(GroundItemAppearedEventDTO{
      gi.id, gi.itemId, static_cast<int16_t>(gi.x),
      static_cast<int16_t>(gi.y)});

  broadcastInventoryUpdate(player);
}

void InventoryManager::takeItem(uint32_t playerId) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;

  Character &player = *it->second;

  for (auto git = groundItems.begin(); git != groundItems.end(); ++git) {
    if (!isOnGroundItem(player.getX(), player.getY(), *git))
      continue;

    if (!player.addItem(git->itemId))
      continue;

    uint32_t removedId = git->id;
    groundItems.erase(git);

    messagesToSend.push_back(GroundItemRemovedEventDTO{removedId});
    broadcastInventoryUpdate(player);
    return;
  }
}

void InventoryManager::broadcastInventoryUpdate(Character &player) {
  messagesToSend.push_back(InventoryUpdateEventDTO{
      player.getId(), player.getInventoryItems(),
      player.getEquippedWeapon(), player.getEquippedArmor(),
      player.getEquippedHelmet(), player.getEquippedShield()});
}

void InventoryManager::broadcastPlayerInfo(Character &player) {
  messagesToSend.push_back(PlayerInfoEventDTO{
      player.getId(), player.getHp(), player.getMaxHp(), player.getMana(),
      player.getMaxMana(), player.getGold(), player.getLevel(),
      player.getExperience()});
}

void InventoryManager::consumePotion(Character &player, const ItemDef &def) {
  if (def.type == ItemType::PotionHp) {
    player.heal(def.healAmount);
  } else if (def.type == ItemType::PotionMana) {
    player.addMana(def.healAmount);
  }
}

const std::vector<InventoryManager::GroundItem> &
InventoryManager::getGroundItems() const {
  return groundItems;
}

void InventoryManager::addGroundItem(uint8_t itemId, int x, int y) {
  GroundItem gi;
  gi.id = nextGroundItemId++;
  gi.itemId = itemId;
  gi.x = x;
  gi.y = y;
  groundItems.push_back(gi);

  messagesToSend.push_back(GroundItemAppearedEventDTO{
      gi.id, gi.itemId, static_cast<int16_t>(gi.x),
      static_cast<int16_t>(gi.y)});
}

bool InventoryManager::isOnGroundItem(int px, int py,
                                       const GroundItem &gi) const {
  return gi.x >= px && gi.x < px + Character::ANCHO && gi.y >= py &&
         gi.y < py + Character::ALTO;
}
