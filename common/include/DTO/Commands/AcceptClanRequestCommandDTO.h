#ifndef ACCEPT_CLAN_REQUEST_COMMAND_DTO_H
#define ACCEPT_CLAN_REQUEST_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct AcceptClanRequestCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::AcceptClanRequestCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t founderId;
  std::string playerName;
};

#endif
