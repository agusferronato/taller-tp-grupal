#ifndef GROUND_ITEM_REMOVED_EVENT_DTO_H
#define GROUND_ITEM_REMOVED_EVENT_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct GroundItemRemovedEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::GroundItemRemovedEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t groundItemId;
};

#endif
