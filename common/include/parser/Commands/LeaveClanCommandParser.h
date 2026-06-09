#ifndef LEAVE_CLAN_COMMAND_PARSER_H
#define LEAVE_CLAN_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class LeaveClanCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
