#ifndef REGISTER_PLAYER_EVENT_DTO_H
#define REGISTER_PLAYER_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

enum class RegisterStatus : uint8_t {
  Success = 0,
  PlayerAlreadyExists = 1,
};

struct RegisterPlayerEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::RegisterPlayerEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  RegisterStatus status;
};

#endif // REGISTER_PLAYER_EVENT_DTO_H
