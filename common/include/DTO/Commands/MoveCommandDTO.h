#ifndef MOVE_COMMAND_DTO_H
#define MOVE_COMMAND_DTO_H

#include "Direction.h"
#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct MoveCommandDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::MoveCommand;

public:
  CommandOpCode getCode() const { return opCode; }

  uint32_t player_id;
  Direction direction;
};

#endif
