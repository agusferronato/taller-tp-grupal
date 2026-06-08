#ifndef KICK_CLAN_MEMBER_COMMAND_DTO_H
#define KICK_CLAN_MEMBER_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct KickClanMemberCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::KickClanMemberCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t founderId;
  std::string playerName;
};

#endif
