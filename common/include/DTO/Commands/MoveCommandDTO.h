#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "Direction.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct MoveCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::MoveCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }

  uint32_t playerId;
  Direction direction;
};

#endif
