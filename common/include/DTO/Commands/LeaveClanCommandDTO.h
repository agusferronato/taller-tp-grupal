#ifndef LEAVE_CLAN_COMMAND_DTO_H
#define LEAVE_CLAN_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct LeaveClanCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::LeaveClanCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
