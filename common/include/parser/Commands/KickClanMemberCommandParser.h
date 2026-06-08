#ifndef KICK_CLAN_MEMBER_COMMAND_PARSER_H
#define KICK_CLAN_MEMBER_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class KickClanMemberCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
