#ifndef REGISTER_PLAYER_RESPONSE_DTO_H
#define REGISTER_PLAYER_RESPONSE_DTO_H

#include <cstdint>

struct RegisterPlayerResponseDTO {
  uint32_t playerId;
  uint8_t status;
};

#endif // REGISTER_PLAYER_RESPONSE_DTO_H
