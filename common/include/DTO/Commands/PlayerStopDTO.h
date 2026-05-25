#ifndef PLAYER_STOP_DTO_H
#define PLAYER_STOP_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerStopDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::PlayerStop;

public:
  CommandOpCode getCode() const { return opCode; }
  uint32_t player_id;
};

#endif
