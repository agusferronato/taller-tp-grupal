#ifndef LOGIN_RESULT_EVENT_DTO_H
#define LOGIN_RESULT_EVENT_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

enum class LoginStatus : uint8_t {
  Success = 0,
  PlayerNotFound = 1,
  AlreadyOnline = 2,
};

struct LoginResultEventDTO {
private:
  static constexpr EventOpcode opCode = EventOpcode::LoginResultEvent;

public:
  EventOpcode getCode() const { return opCode; }
  uint32_t playerId;
  LoginStatus status;
};

#endif
