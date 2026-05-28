#ifndef UNEQUIP_COMMAND_DTO_H
#define UNEQUIP_COMMAND_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct UnequipCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::UnequipCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t equipSlot;
};

#endif
