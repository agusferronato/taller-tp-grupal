#ifndef GROUND_ITEMS_LIST_EVENT_DTO_H
#define GROUND_ITEMS_LIST_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <vector>

struct GroundItemInfoDTO {
  uint32_t groundItemId;
  uint8_t itemId;
  int16_t x;
  int16_t y;
};

struct GroundItemsListEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::GroundItemsListEvent;

public:
  EventOpcode getCode() const { return opCode; }
  std::vector<GroundItemInfoDTO> items;
};

#endif
