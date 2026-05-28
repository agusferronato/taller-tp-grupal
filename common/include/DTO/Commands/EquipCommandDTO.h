#ifndef EQUIP_COMMAND_DTO_H
#define EQUIP_COMMAND_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct EquipCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::EquipCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t inventorySlot;
};

#endif
