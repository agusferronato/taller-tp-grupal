#ifndef DROP_ITEM_COMMAND_DTO_H
#define DROP_ITEM_COMMAND_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct DropItemCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::DropItemCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t inventorySlot;
};

#endif
