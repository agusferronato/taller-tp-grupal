#ifndef LOGIN_PLAYER_COMMAND_DTO_H
#define LOGIN_PLAYER_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct LoginPlayerCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::LoginPlayerCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  std::string name;
};

#endif
