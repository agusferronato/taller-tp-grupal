#ifndef EXIT_COMMAND_DTO_H
#define EXIT_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct ExitCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::ExitCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
