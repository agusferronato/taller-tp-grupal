#ifndef REGISTER_PLAYER_H
#define REGISTER_PLAYER_H

#include "protocol/ProtocolCodes.h"
#include <string>

struct RegisterPlayerDTO {
private:
  static constexpr CommandOpCode opCode = CommandOpCode::RegisterPlayer;

public:
  CommandOpCode getCode() const { return opCode; }
  std::string name;
};

#endif
