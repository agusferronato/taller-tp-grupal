#ifndef GROUND_ITEM_MANAGER_H
#define GROUND_ITEM_MANAGER_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "GroundItemsListEventDTO.h"

class GroundItemManager {
public:
  using GroundItem = GroundItemInfoDTO;

  void add(uint32_t id, uint8_t itemId, int x, int y);
  void remove(uint32_t id);
  void setAll(const std::vector<GroundItemInfoDTO> &items);
  void clear();

  const std::unordered_map<uint32_t, GroundItem> &getAll() const;

private:
  std::unordered_map<uint32_t, GroundItem> items;
};

#endif
