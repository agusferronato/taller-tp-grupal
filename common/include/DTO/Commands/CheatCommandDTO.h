#ifndef CHEAT_COMMAND_DTO_H
#define CHEAT_COMMAND_DTO_H

#include "CheatType.h"
#include "protocol/ProtocolCodes.h"

struct CheatCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::CheatCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }

  CheatType cheat;
};

#endif
