#ifndef LOGIN_PLAYER_DTO_H
#define LOGIN_PLAYER_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct LoginPlayerDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::LoginPlayer;

public:
  CommandOpCode getCode() const { return opCode; }
  std::string name;
};

#endif
