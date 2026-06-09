#ifndef VALIDATE_LOGIN_COMMAND_DTO_H
#define VALIDATE_LOGIN_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct ValidateLoginCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::ValidateLoginCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  std::string playerName;
};

#endif
