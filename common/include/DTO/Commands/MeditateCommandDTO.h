#ifndef MEDITATE_COMMAND_DTO_H
#define MEDITATE_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct MeditateCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::MeditateCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
