#ifndef REGISTER_PLAYER_RESPONSE_DTO_H
#define REGISTER_PLAYER_RESPONSE_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct RegisterPlayerResponseDTO {
private:
  static constexpr ServerOpcode opCode = ServerOpcode::RegisterResponse;

public:
  ServerOpcode getCode() const { return opCode; }
  uint32_t playerId;
  uint8_t status;
};

#endif // REGISTER_PLAYER_RESPONSE_DTO_H
