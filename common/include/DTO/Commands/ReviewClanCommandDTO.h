#ifndef REVIEW_CLAN_COMMAND_DTO_H
#define REVIEW_CLAN_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>

struct ReviewClanCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::ReviewClanCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
};

#endif
