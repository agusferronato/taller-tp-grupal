#ifndef INVENTORY_UPDATE_EVENT_DTO_H
#define INVENTORY_UPDATE_EVENT_DTO_H

#include <array>
#include <cstdint>

#include "InventoryConstants.h"
#include "protocol/ProtocolCodes.h"

struct InventoryUpdateEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::InventoryUpdateEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  std::array<uint8_t, MAX_INVENTORY_SLOTS> items{};
  uint8_t equippedWeapon{0};
  uint8_t equippedArmor{0};
  uint8_t equippedHelmet{0};
  uint8_t equippedShield{0};
};

#endif
