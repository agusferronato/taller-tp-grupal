#ifndef GROUND_ITEM_APPEARED_EVENT_DTO_H
#define GROUND_ITEM_APPEARED_EVENT_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct GroundItemAppearedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::GroundItemAppearedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t groundItemId;
  uint8_t itemId;
  int16_t x;
  int16_t y;
};

#endif
