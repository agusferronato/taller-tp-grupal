#ifndef REJECT_CLAN_REQUEST_COMMAND_PARSER_H
#define REJECT_CLAN_REQUEST_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class RejectClanRequestCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
