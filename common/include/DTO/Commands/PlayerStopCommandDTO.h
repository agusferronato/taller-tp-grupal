#ifndef PLAYER_STOP_DTO_H
#define PLAYER_STOP_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerStopCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::PlayerStopCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
