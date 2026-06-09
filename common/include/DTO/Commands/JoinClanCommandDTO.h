#ifndef JOIN_CLAN_COMMAND_DTO_H
#define JOIN_CLAN_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct JoinClanCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::JoinClanCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t playerId;
  std::string clanName;
};

#endif
