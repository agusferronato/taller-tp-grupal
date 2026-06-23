#ifndef REJECT_CLAN_REQUEST_COMMAND_DTO_H
#define REJECT_CLAN_REQUEST_COMMAND_DTO_H

#include "protocol/ProtocolCodes.h"
#include <cstdint>
#include <string>

struct RejectClanRequestCommandDTO {
private:
  static constexpr ClientCommandOpCode opCode =
      ClientCommandOpCode::RejectClanRequestCommand;

public:
  ClientCommandOpCode getCode() const { return opCode; }
  uint32_t founderId;
  std::string playerName;
};

#endif
