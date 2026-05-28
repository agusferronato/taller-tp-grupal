#ifndef TAKE_ITEM_COMMAND_DTO_H
#define TAKE_ITEM_COMMAND_DTO_H

#include <cstdint>

#include "protocol/ProtocolCodes.h"

struct TakeItemCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::TakeItemCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
