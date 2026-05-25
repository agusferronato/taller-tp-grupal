#ifndef PLAYER_STOPPED_DTO_H
#define PLAYER_STOPPED_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct PlayerStoppedDTO {
private:
  static constexpr ServerOpcode opCode = ServerOpcode::PlayerStopped;

public:
  ServerOpcode getCode() const { return opCode; }
  uint32_t player_id;
};

#endif
