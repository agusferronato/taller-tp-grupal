#include "GroundItemManager.h"

void GroundItemManager::add(uint32_t id, uint8_t itemId, int x, int y) {
  items[id] = GroundItem{id, itemId, static_cast<int16_t>(x),
                          static_cast<int16_t>(y)};
}

void GroundItemManager::remove(uint32_t id) { items.erase(id); }

void GroundItemManager::setAll(const std::vector<GroundItemInfoDTO> &newItems) {
  items.clear();
  for (const auto &item : newItems) {
    items[item.groundItemId] = item;
  }
}

void GroundItemManager::clear() { items.clear(); }

const std::unordered_map<uint32_t, GroundItemManager::GroundItem> &
GroundItemManager::getAll() const {
  return items;
}
