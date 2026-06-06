#ifndef ATACK_COMMAND_DTO_H
#define ATACK_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct AtackCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::AtackCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }

  uint32_t playerId;
  int16_t x;
  int16_t y;
};

#endif
