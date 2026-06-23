#include "InventoryManager.h"

#include "DTO/Events/GroundItemAppearedEventDTO.h"
#include "DTO/Events/GroundItemRemovedEventDTO.h"
#include "DTO/Events/InventoryUpdateEventDTO.h"
#include "DTO/Events/PlayerInfoEventDTO.h"
#include "InventoryConstants.h"
#include "ItemData.h"

InventoryManager::InventoryManager(
    std::unordered_map<uint32_t, std::unique_ptr<Character>> &players,
    std::list<ServerEventDTO> &messagesToSend,
    std::vector<GroundItem> &groundItems)
    : players(players), messagesToSend(messagesToSend),
      groundItems(groundItems) {}

void InventoryManager::equipItem(uint32_t playerId, uint8_t slotIndex) {
  auto it = players.find(playerId);
  if (it == players.end())
    return;

  Character &player = *it->second;
  uint8_t itemId = player.getInventory().getItemId(slotIndex);
  if (itemId == EMPTY_ITEM)
    return;

  auto &idata = ItemData::instance();

  if (idata.isPotionHp(itemId) || idata.isPotionMana(itemId)) {
    if (!player.removeItem(slotIndex))
      return;
    consumePotion(player, itemId);
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

  if (!player.removeItem(slotIndex))
    return;

  GroundItem gi;
  gi.id = nextGroundItemId++;
  gi.itemId = itemId;
  gi.x = player.getX();
  gi.y = player.getY();
  groundItems.push_back(gi);

  messagesToSend.push_back(
      GroundItemAppearedEventDTO{gi.id, gi.itemId, static_cast<int16_t>(gi.x),
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
      player.getEquippedWeapon().getID(), player.getEquippedArmor().getID(),
      player.getEquippedHelmet().getID(), player.getEquippedShield().getID()});
}

void InventoryManager::broadcastPlayerInfo(Character &player) {
  messagesToSend.push_back(PlayerInfoEventDTO{
      player.getId(), player.getHp(), player.getMaxHp(), player.getMana(),
      player.getMaxMana(), player.getGold(), player.getLevel(),
      player.getExperience()});
}

void InventoryManager::consumePotion(Character &player, uint8_t itemId) {
  auto &idata = ItemData::instance();
  if (idata.isPotionHp(itemId)) {
    player.heal(idata.getPotionData(itemId).healAmount);
  } else if (idata.isPotionMana(itemId)) {
    player.addMana(idata.getPotionData(itemId).healAmount);
  }
}

void InventoryManager::addGroundItem(uint8_t itemId, int x, int y) {
  GroundItem gi;
  gi.id = nextGroundItemId++;
  gi.itemId = itemId;
  gi.x = x;
  gi.y = y;
  groundItems.push_back(gi);

  messagesToSend.push_back(
      GroundItemAppearedEventDTO{gi.id, gi.itemId, static_cast<int16_t>(gi.x),
                                 static_cast<int16_t>(gi.y)});
}

bool InventoryManager::isOnGroundItem(int px, int py,
                                      const GroundItem &gi) const {
  constexpr int margin = 16;
  return gi.x >= px - margin && gi.x < px + Character::ANCHO + margin &&
         gi.y >= py - margin && gi.y < py + Character::ALTO + margin;
}
