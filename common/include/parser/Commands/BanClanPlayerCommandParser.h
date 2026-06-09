#ifndef BAN_CLAN_PLAYER_COMMAND_PARSER_H
#define BAN_CLAN_PLAYER_COMMAND_PARSER_H

#include "ClientCommandParser.h"
#include <vector>

class BanClanPlayerCommandParser : public ClientCommandParser {
public:
  void serialize(std::vector<uint8_t> &bytes,
                 const ClientCommandDTO &dto) override;

  ClientCommandDTO deserialize(Protocol &protocol) override;
};

#endif
